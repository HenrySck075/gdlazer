#include "OverlayContainer.hpp"
#include "../../Game.hpp"
#include "../../../utils.hpp"
#include "../../input/events/KeyEvent.hpp"
#include "Geode/cocos/keypad_dispatcher/CCKeypadDispatcher.h"

bool OverlayContainer::init() {
  if (!VisibilityContainer::init()) return false;
  main = Container::create();
  main->setContentSize({0,0});
  main->setAnchorPoint({0.5,0.5});
  main->setAnchor(Anchor::Center);
  addChild(main);
  setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
  setCascadeOpacityEnabled(false);
  addListener("keyboardEvent", [this](NodeEvent* e){
    if (static_cast<KeyboardEvent*>(e)->key.key == KEY_Escape) hide();
  });
  addListener("keypadEvent", [this](NodeEvent* e){
    if (static_cast<KeypadEvent*>(e)->key == ccKeypadMSGType::kTypeBackClicked) hide();
  });
  shown.addCallback([this](NodeEvent* e){
    auto g = Game::get();
    if ((bool)shown) {
      log::debug("[OverlayContainer]: show overlay");
      g->pushOverlay(this);
      g->dispatchEvent(new OverlayEvent(this, OverlayEvent::Type::Popin));
    }
    else {
      log::debug("[OverlayContainer]: hide overlay");
      g->popOverlay(this);
      g->dispatchEvent(new OverlayEvent(this, OverlayEvent::Type::Popout));
    }
  });
  return true;
}


void OverlayContainer::onClick(MouseEvent* e) {
  #if 0
  auto j = boundingBoxFromContentSize(main);
  if (!j.containsPoint(e->position)) onDismiss();
  #endif
}