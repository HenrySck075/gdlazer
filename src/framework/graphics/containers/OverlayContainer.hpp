#pragma once

#include "Container.hpp"
#include "../../input/events/KeyEvent.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/keypad_dispatcher/CCKeypadDispatcher.h"

// yeah thats it
// @note Cascade opacity has been disabled
class OverlayContainer : public Container {
protected:
    Container* main;
public:
    bool init() {
        if (!Container::init()) return false;
        main = Container::create();
        main->setContentSize({0,0});
        main->setAnchorPoint({0.5,0.5});
        main->setPositionWithUnit({50,50},Unit::Percent,Unit::Percent);
        addChild(main);
        setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
        setCascadeOpacityEnabled(false);
        setColor({0,0,0,127});
        addListener("keyboardEvent", [this](NodeEvent* e){
            if (static_cast<KeyboardEvent*>(e)->key.key == enumKeyCodes::KEY_Escape) hide();
        });
        addListener("keypadEvent", [this](NodeEvent* e){
            if (static_cast<KeypadEvent*>(e)->key==ccKeypadMSGType::kTypeBackClicked) hide();
        });
        return true;
    }
    virtual void onOpen() = 0;
    virtual void onClose() = 0;

    virtual void show() = 0;
    virtual void hide() {
        if (m_pActionManager->numberOfRunningActionsInTarget(this)!=0) {
            m_pScheduler->scheduleSelector(schedule_selector(OverlayContainer::checkActions),this,0.01f,false);
        }
        else removeFromParent();
    };

    void checkActions(float) {
        if (m_pActionManager->numberOfRunningActionsInTarget(this)==0) {
            removeFromParent();
            m_pScheduler->unscheduleSelector(schedule_selector(OverlayContainer::checkActions), this);
        }
    }

    void onClick(MouseEvent* e) override {
        auto j = boundingBoxFromContentSize(main);
        if (!j.containsPoint(e->position)) onDismiss();
    }
    virtual void onDismiss() {hide();}
};
