#pragma once
#include "../graphics/containers/Container.hpp"
#include "../../utils.hpp"

class ScreenTransitionEvent;
#include "ScreenTransitionEvent.hpp"

// idgaf imo
class Screen : public Container {
public:
    static Screen* create() {
        create_class(Screen, init);
    };
    bool init() {
        Container::init();
        setContentSizeWithUnit(CCSize(1,1),Unit::Viewport,Unit::Viewport);
        return true;
    };
    virtual void onEntering(ScreenTransitionEvent e) {};
    virtual void onExiting(ScreenTransitionEvent e) {};

    // when?
    virtual void onSuspending(ScreenTransitionEvent e) {};
    virtual void onResuming(ScreenTransitionEvent e) {};
};