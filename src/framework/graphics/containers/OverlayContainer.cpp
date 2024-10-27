#include "OverlayContainer.hpp"
#include "../../Game.hpp"

bool OverlayContainer::init() {
    if (!VisibilityContainer::init()) return false;
    main = Container::create();
    main->setContentSize({0,0});
    main->setAnchorPoint({0.5,0.5});
    main->setPositionWithUnit({50,50},Unit::Percent,Unit::Percent);
    addChild(main);
    setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
    setCascadeOpacityEnabled(false);
    addListener("keyboardEvent", [this](NodeEvent* e){
        if (static_cast<KeyboardEvent*>(e)->key.key == KEY_Escape) hide();
    });
    addListener("keypadEvent", [this](NodeEvent* e){
        if (static_cast<KeypadEvent*>(e)->key == kTypeBackClicked) hide();
    });
    shown.addCallback([this](NodeEvent* e){
        log::debug("[OverlayContainer]: {}", shown.operator bool());
        if ((bool)shown) {
            log::debug("[OverlayContainer]: show overlay");
            if (IsDebuggerPresent()) DebugBreak();
            Game::get()->pushOverlay(this);
        }
        else {
            log::debug("[OverlayContainer]: hide overlay");
            Game::get()->popOverlay(this);
        }
    });
    return true;
}

