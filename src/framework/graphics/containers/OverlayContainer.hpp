#pragma once

#include "VisibilityContainer.hpp"
#include "../../input/events/KeyEvent.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/keypad_dispatcher/CCKeypadDispatcher.h"

// yeah thats it
// @note Cascade opacity has been disabled
class OverlayContainer : public VisibilityContainer {
protected:
    Container* main;
public:
    bool init();
    void onClose() override;
    void checkActions(float);

    void onClick(MouseEvent* e) override {
        auto j = boundingBoxFromContentSize(main);
        if (!j.containsPoint(e->position)) onDismiss();
    }
    virtual void onDismiss() {hide();}
};
