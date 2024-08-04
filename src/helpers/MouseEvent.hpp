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
    Enter, Exit, Click, MouseDown, MouseUp, Move
};

class MouseFilter : public EventFilter<MouseEvent> {
protected:
    bool m_entered = false;
    CCNode* m_target;
    bool m_keepPropagating;
public:

    using Callback = bool(MouseType, CCPoint);

    ListenerResult handle(MiniFunction<Callback> fn, MouseEvent* event);
    MouseFilter(CCNode* target, bool keepPropagating) {
        m_target = target;
        auto j = static_cast<CCBool*>(m_target->getUserObject("clicking"_spr));
        if (j == nullptr) {
            m_target->setUserObject("clicking"_spr, CCBool::create(false));
        }
        m_keepPropagating = keepPropagating;
        //log::debug("[MouseFilter]: {} | {}", CCDirector::sharedDirector()->getVisibleSize(), CCDirector::sharedDirector()->getWinSizeInPixels());
    };
};


#endif
