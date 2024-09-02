#pragma once

#include "Container.hpp"

// yeah thats it
class OverlayContainer : public Container {
public:
    virtual void onOpen() = 0;
    virtual void onClose() = 0;
};
