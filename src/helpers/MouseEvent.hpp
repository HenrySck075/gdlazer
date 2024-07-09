#ifndef __osu_helpers_mouse_event__
#define __osu_helpers_mouse_event__

#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>

using namespace geode::prelude;

class MouseMoveEvent : public Event {

protected:
    CCPoint m_location;

public:
    MouseMoveEvent(CCPoint const& location) {
        m_location = location;
    };

    CCPoint getLocation() const { return this->m_location; };
};


class MouseMoveFilter : public EventFilter<MouseMoveEvent> {
protected:
    CCNode* m_target;
    bool m_keepPropangating;
public:
    
    using Callback = void(CCPoint);

    ListenerResult handle(MiniFunction<Callback> fn, MouseMoveEvent* event);
    MouseMoveFilter() {};
    MouseMoveFilter(CCNode* target, bool keepPropangating) {
        m_target = target;
        m_keepPropangating = keepPropangating;
    };
};

enum MouseMoveType {
    Enter, Exit
};

class MouseEnterExitFilter : public EventFilter<MouseMoveEvent> {
protected:
    bool m_entered = false;
    CCNode* m_target;
    bool m_keepPropangating;
public:

    using Callback = void(MouseMoveType, CCPoint);

    ListenerResult handle(MiniFunction<Callback> fn, MouseMoveEvent* event);
    MouseEnterExitFilter(CCNode* target, bool keepPropangating) {
        m_target = target;
        m_keepPropangating = keepPropangating;
    };
};
#endif