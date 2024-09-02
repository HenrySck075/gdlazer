#pragma once

#include <Geode/loader/Event.hpp>
#include <Geode/cocos/include/cocos2d.h>
class ActionFinished : public geode::Event {
public:
    cocos2d::CCNode* m_target;
    cocos2d::CCAction* m_action;

    ActionFinished(cocos2d::CCNode* target, cocos2d::CCAction* action) : m_target(target), m_action(action) {}
};

class ActionFinishedFilter : geode::EventFilter<ActionFinished> {
public:
    cocos2d::CCNode* m_target;
    
    using Callback = void(cocos2d::CCNode* target, cocos2d::CCAction* action);
    ActionFinishedFilter(cocos2d::CCNode* target) : m_target(target) {};
};
