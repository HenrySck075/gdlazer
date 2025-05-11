#include "OverlayContainer.hpp"
#include "../../Game.hpp"
#include "../../input/events/KeyEvent.hpp"
#include "../../input/events/MouseEvent.hpp"

GDF_NS_START
bool OverlayContainer::init() {
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
  
  m_shown.addCallback([this](bool, bool value){
    auto g = Game::get();
    if (value) {
      geode::log::debug("[OverlayContainer]: show overlay");
      g->pushOverlay(this);
      g->dispatchEvent(new OverlayEvent(this, OverlayEvent::Type::Popin));
    }
    else {
      geode::log::debug("[OverlayContainer]: hide overlay");
      g->popOverlay(this);
      g->dispatchEvent(new OverlayEvent(this, OverlayEvent::Type::Popout));
    }
  });
  return true;
}
GDF_NS_END
