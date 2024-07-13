#ifndef __osu_helpers_mouse_event__
#define __osu_helpers_mouse_event__

#include <Geode/Geode.hpp>
#include "../utils.hpp"

using namespace geode::prelude;

class MouseEvent : public Event {

protected:
    CCPoint m_location;

public:
    MouseEvent(CCPoint const& location) {
        m_location = location;
    };

    CCPoint getLocation() const { return this->m_location; };
};

enum MouseType {
    Enter, Exit, Click
};

class MouseFilter : public EventFilter<MouseEvent> {
protected:
    bool m_entered = false;
    CCNode* m_target;
    bool m_keepPropangating;
public:

    using Callback = void(MouseType, CCPoint);

    ListenerResult handle(MiniFunction<Callback> fn, MouseEvent* event);
    MouseFilter(CCNode* target, bool keepPropangating) {
        m_target = target;
        m_keepPropangating = keepPropangating;
        log::debug("[MouseFilter]: {} | {}", CCDirector::sharedDirector()->getVisibleSize(), CCDirector::sharedDirector()->getWinSizeInPixels());
    };
};

#endif
