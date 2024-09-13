#pragma once

#include "Container.hpp"

// yeah thats it
class OverlayContainer : public Container {
protected:
    Container* main;
public:
    bool init() {
        if (!Container::init()) return false;
        main = Container::create();
        setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
        return true;
    }
    virtual void onOpen() = 0;
    virtual void onClose() = 0;

    virtual void show() = 0;
    virtual void hide() = 0;
};
