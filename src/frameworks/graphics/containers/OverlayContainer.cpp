#include "OverlayContainer.hpp"
#include "../../Game.hpp"
#include "../../input/events/KeyEvent.hpp"
#include "../../input/events/MouseEvent.hpp"
#include "ContainerActions.hpp"

GDF_NS_START
bool OverlayContainer::init() {
  
  m_shown.addCallback([this](bool, bool value){
    auto g = Game::get();
    if (value) {
      g->pushOverlay(this);
      g->dispatchEvent(new OverlayEvent(this, OverlayEvent::Type::Popin));
    }
    else {
      g->dispatchEvent(new OverlayEvent(this, OverlayEvent::Type::Popout));
      g->popOverlay(this);
    }
  });
  if (!VisibilityContainer::init()) return false;
  
  m_main = Container::create();
  m_main->setContentSize({0,0});
  m_main->setAnchorPoint({0.5,0.5});
  m_main->setAnchor(geode::Anchor::Center);
  addChild(m_main);
  setContentSize({100,100},Unit::Percent,Unit::Percent);
  setCascadeOpacityEnabled(false);
  addListener<KeyEvent>([this](KeyEvent* e){
    if (e->m_key == KEY_Escape) hide();
    return true;
  });
  /*
  addListener("keypadEvent", [this](NodeEvent* e){
    if (static_cast<KeypadEvent*>(e)->key == ccKeypadMSGType::kTypeBackClicked) hide();
  });
  */
  addListener<MouseEvent>([this](MouseEvent* e){
    if (e->m_eventType == MouseEventType::Click) {
      #if 0
      auto j = boundingBoxFromContentSize(main);
      if (!j.containsPoint(e->position)) onDismiss();
      #endif
    }
    return true;
  });
  return true;
}

void OverlayContainer::show() {
  VisibilityContainer::show();
  m_blockingEvents = m_pActionManager->numberOfRunningActionsInTarget(this) != 0;
  ccColor4B bgColor;
  if (auto bgColorSaved = typeinfo_cast<CCDataHolder<ccColor4B>*>(this->getUserObject("gdlazer/overlay/bgColor"))) {
    bgColor = bgColorSaved->getData();
  } else {
    bgColor = this->getBackgroundColor();
    this->setUserObject(
      "gdlazer/overlay/bgColor", 
      CCDataHolder<ccColor4B>::create(bgColor)
    );
  }
  this->setBackgroundColor({bgColor.r, bgColor.g, bgColor.b, 0});
  this->runAction(
    ContainerTintOpacityTo::create(0.1f, bgColor.a)
  );
}

void OverlayContainer::hide() {
  VisibilityContainer::hide();
  runAction(
    ContainerTintOpacityTo::create(0.1f, 0)
  );
  m_blockingEvents = true;
}

bool OverlayContainer::doDispatchEvent(Event* event, std::type_index type) {
  if (m_blockingEvents && type != typeid(NodeSizeUpdated)) return true;
  return VisibilityContainer::doDispatchEvent(event, type);
}
void OverlayContainer::update(float) {
  if (m_blockingEvents) {
    m_blockingEvents = m_pActionManager->numberOfRunningActionsInTarget(this) != 0;
  }
};
GDF_NS_END
