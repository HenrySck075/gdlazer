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
protected:
    // title
    std::string m_title = "";
public:
    std::string title() {return m_title;}
    void title(std::string newTitle) {m_title = newTitle;}

    static Screen* create() {
        create_class(Screen, init);
    };
    bool init() override {
        if (!Container::init()) return false;
        setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
        return true;
    };
    virtual void onEntering(ScreenTransitionEvent e) {};
    virtual void onExiting(ScreenTransitionEvent e) {};

    // when?
    virtual void onSuspending(ScreenTransitionEvent e) {};
    virtual void onResuming(ScreenTransitionEvent e) {};
};