#include "OverlayContainer.hpp"

bool OverlayContainer::init() {
    if (!Container::init()) return false;
    main = Container::create();
    main->setContentSize({0,0});
    main->setAnchorPoint({0.5,0.5});
    main->setPositionWithUnit({50,50},Unit::Percent,Unit::Percent);
    addChild(main);
    setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
    setCascadeOpacityEnabled(false);
    addListener("keyboardEvent", [this](NodeEvent* e){
        if (static_cast<KeyboardEvent*>(e)->key.key == enumKeyCodes::KEY_Escape) hide();
    });
    addListener("keypadEvent", [this](NodeEvent* e){
        if (static_cast<KeypadEvent*>(e)->key==ccKeypadMSGType::kTypeBackClicked) hide();
    });
    shown.addCallback([this](ValueChangedEvent<bool>* e){
        if (e) onOpen();
        else {
            onClose(); 
            if (m_pActionManager->numberOfRunningActionsInTarget(this)!=0) {
                m_pScheduler->scheduleSelector(
                    schedule_selector(OverlayContainer::checkActions),this,1,false
                );
            }
            else removeFromParent();
        }
    });
    return true;
}