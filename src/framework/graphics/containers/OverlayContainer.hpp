#pragma once

#include "Container.hpp"
#include "../../input/events/KeyEvent.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/keypad_dispatcher/CCKeypadDispatcher.h"
#include "../../bindables/Bindable.hpp"

// yeah thats it
// @note Cascade opacity has been disabled
class OverlayContainer : public Container {
protected:
    Container* main;
    Bindable<bool> shown = false;
public:
    bool init();
    virtual void onOpen() {};
    virtual void onClose() {};

    bool isOpen() {return shown;}

    virtual void show();
    virtual void hide();

    void checkActions(float) {
        log::debug("ballin'");
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
