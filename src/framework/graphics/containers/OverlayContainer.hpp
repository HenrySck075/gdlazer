#pragma once

#include "Container.hpp"

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
        setColor({0,0,0});
        return true;
    }
    virtual void onOpen() = 0;
    virtual void onClose() = 0;

    virtual void show() = 0;
    virtual void hide() = 0;

    void onClick(MouseEvent* e) override {
        auto j = boundingBoxFromContentSize(main);
        log::debug("[OverlayContainer]: {} | {}", j, j.containsPoint(e->position));
        if (!j.containsPoint(e->position)) onDismiss();
    }
    virtual void onDismiss() {hide();}
};
