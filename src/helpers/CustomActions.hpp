#pragma once

#include <Geode/Geode.hpp>
#include "../utils.hpp"

using namespace geode::prelude;

typedef void (CCObject::*CallFuncP)(float);
#define callfuncp_selector(...) (CallFuncP)(&__VA_ARGS__)

class CCCallFuncP : public CCActionInterval {
private:
    bool m_reversed;
    float m_to;
    float m_from;
    CCObject* m_target;
    CallFuncP m_pCallFunc;

    void update(float time);
    void execute(float percentage);
    /// <summary>
    /// Initializes the action
    /// </summary>
    /// <param name="from">| the starting percentage</param>
    /// <param name="to">| the ending percentage</param>
    /// <param name="duration">| duration of the action</param>
    /// <param name="selector">| the function</param>
    /// <returns>bool</returns>
    bool initWithACatgirl(float from, float to, float duration, CCObject* target, CallFuncP selector);
public:
    static CCCallFuncP* create(float from, float to, float duration, CCObject* target, CallFuncP selector);
    CCCallFuncP* reverse();
};

class CCActionSkip : public CCActionEase {
private:
    float m_fStartTime;

    bool initWithACatgirl(CCActionInterval* action, float startTime);
    void update(float time);
public:
    CCObject* copyWithZone(CCZone* pZone);
    ~CCActionSkip();
    static CCActionSkip* create(float startTime, CCActionInterval* action);
};


/** @brief Sizes a CCNode object to a zoom factor by modifying it's contentSize attribute.
 @warning This action doesn't support "reverse"
 */
class CCResizeTo : public CCActionInterval
{
public:
    /** initializes the action with the same scale factor for X and Y */
    bool initWithDuration(float duration, float s);

    /** initializes the action with and X factor and a Y factor */
    bool initWithDuration(float duration, float sx, float sy);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode* pTarget);
    virtual void update(float time);

public:

    /** creates the action with the same scale factor for X and Y */
    static CCResizeTo* create(float duration, float s);

    /** creates the action with and X factor and a Y factor */
    static CCResizeTo* create(float duration, float sx, float sy);
protected:
    float m_fContentWidth;
    float m_fContentHeight;
    float m_fStartContentWidth;
    float m_fStartContentHeight;
    float m_fEndContentWidth;
    float m_fEndContentHeight;
    float m_fDeltaX;
    float m_fDeltaY;
};
// CCActionEase, done right!
// Q: why dont you hook ActionEase
// A: idk
class CCEaseOutQuart : public CCEaseOut {
public:
    static CCEaseOutQuart* create(CCActionInterval* action) {
        create_class(CCEaseOutQuart, initWithAction, action, 4);
    }
    void update(float time) override {
        m_pInner->update(1-powf(1-time,4));
    }
};
class CCEaseOutQuint : public CCEaseOut {
public:
    static CCEaseOutQuint* create(CCActionInterval* action) {
        create_class(CCEaseOutQuint, initWithAction, action, 5);
    }
    void update(float time) override {
        m_pInner->update(1-powf(1-time,5));
    }
};

typedef std::function<void()> SEL_CallFuncL;

/** @brief Calls a 'callback'
*/
class CC_DLL CCCallFuncL : public CCActionInstant //<NSCopying>
{
public:
    /**
     *  @js ctor
     */
    CCCallFuncL()
        : m_nScriptHandler(0)
        , m_pCallFunc(NULL)
    {
    }
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCCallFuncL();

    /** creates the action with the callback 

    * typedef void (CCObject::*SEL_CallFunc)();
    * @lua NA
    */
    static CCCallFuncL * create(SEL_CallFuncL selector);

    /** super methods
     * @lua NA
     */
    virtual void update(float time);
    /**
     * @js  NA
     * @lua NA
     */
    CCObject * copyWithZone(CCZone *pZone);
    /**
     * @lua NA
     */
    inline int getScriptHandler() { return m_nScriptHandler; };
protected:
    SEL_CallFuncL m_pCallFunc;
    int m_nScriptHandler;

};

// MoveFromTo

/// @brief CCMoveTo but with the starting position manually defined.
///
/// Used in ButtonArea because idk how the heck did i break it
class CCMoveFromTo : public CCMoveTo {
public:
    bool initWithDuration(float duration, const CCPoint& startPos, const CCPoint& endPos) {
        if (!CCMoveTo::initWithDuration(duration, endPos)) return false;
        m_startPosition = startPos;
        return true;
    };
    static CCMoveFromTo* create(float duration, const CCPoint& startPos, const CCPoint& endPos) {
        create_class(CCMoveFromTo, initWithDuration, duration, startPos, endPos);
    }
    void update(float t) {
        if (m_pTarget) {
            m_pTarget->setPosition(m_startPosition + (m_positionDelta * t));
        }
    }
    void startWithTarget(CCNode *pTarget) {
        CCActionInterval::startWithTarget(pTarget);
        m_previousPosition = m_startPosition;
        m_positionDelta = ccpSub( m_endPosition, m_startPosition );
    }
};

class CCRepeatUntil : public CCRepeatForever {
private:
    geode::utils::MiniFunction<bool()> m_predicate;
    bool m_predicateCheckValid = false;
    CCAction* m_pAction;
public:
    bool initWithPredicate(CCAction* action, decltype(m_predicate) predicate) {
        m_pAction = action;
        m_predicate = predicate;
        return true;
    }
    static CCRepeatUntil* create(CCAction* action, decltype(m_predicate) predicate) {
        create_class(CCRepeatUntil, initWithPredicate, action, predicate);
    }
    void step(float dt) {
        m_pAction->step(dt);
        if (!m_predicateCheckValid) {
            m_predicateCheckValid = m_predicate(); 
        }
        if (m_pAction->isDone())
        {
            if (m_predicateCheckValid) return;
            float diff = m_pInnerAction->getElapsed() - m_pInnerAction->getDuration();
            m_pAction->startWithTarget(m_pTarget);
            // to prevent jerk. issue #390, 1247
            m_pAction->step(0.0f);
            m_pAction->step(diff);
        }
    }
    bool isDone() override {
        return m_pInnerAction->isDone() && m_predicateCheckValid;
    }
};

class CCWaitUntil : public CCFiniteTimeAction {
    CCAction* m_pInner;
public:
    bool initWithAction(CCAction* action) {
        m_pInner = action;
        return true;
    }
    static CCWaitUntil* create(CCAction* action) {
        create_class(CCWaitUntil, initWithAction, action);
    }
    bool isDone() override {
        return m_pInner->isDone();
    }
    void stop() override {
        m_pInner->stop();
    }
    CCWaitUntil* reverse() override {
        if (auto pInner = typeinfo_cast<CCFiniteTimeAction*>(m_pInner)) {
            if (auto inner = pInner->reverse()) return CCWaitUntil::create(inner);
            else return nullptr;
        } else return CCWaitUntil::create(m_pInner);
    }
};