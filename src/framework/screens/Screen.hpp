#pragma once
#include "..\graphics\containers\Container.hpp"

class ScreenTransitionEvent;
#include "ScreenTransitionEvent.hpp"

// idgaf imo
class Screen : public Container {
public:
    virtual void onEntering(ScreenTransitionEvent e) = 0;
    virtual void onExiting(ScreenTransitionEvent e) = 0;

    // when?
    virtual void onSuspending(ScreenTransitionEvent e) = 0;
    virtual void onResuming(ScreenTransitionEvent e) = 0;
}