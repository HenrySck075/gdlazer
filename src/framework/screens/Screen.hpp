#pragma once
#include "../graphics/containers/Container.hpp"
#include "../../utils.hpp"

class ScreenTransitionEvent;
#include "ScreenTransitionEvent.hpp"

class ScreenTransitionNotifier : public NodeEvent {
public:
    Screen* caller;
    ScreenTransitionNotifier(std::string name, Screen* c) : NodeEvent("og"+name), caller(c) {}
};

// idgaf imo
class Screen : public Container {
public:
    static Screen* create() {
        create_class(Screen, init);
    };
    bool init() override {
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