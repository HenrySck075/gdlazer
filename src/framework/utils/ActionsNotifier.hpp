#pragma once

#include <Geode/loader/Event.hpp>
#include <Geode/cocos/include/cocos2d.h>
using namespace geode::prelude;
class ActionFinished : public geode::Event {
public:
    CCAction* m_action;
    ActionFinished(CCAction* action) : m_action(action) {}
};

class ActionFinishedFilter : geode::EventFilter<ActionFinished> {
    CCNode* m_target;
public:
    using Callback = void(CCAction* action);
    ListenerResult handle(MiniFunction<Callback> fn, ActionFinished* event) {
        if (event->m_action->getTarget() == m_target) {
            fn(event->m_action);
            return ListenerResult::Stop;
        }
        return ListenerResult::Propagate;
    };
    /// @param target Filter the event to only dispatch callbacks if the m_target member is this target
    ActionFinishedFilter(CCNode* target) : m_target(target) {};
};
