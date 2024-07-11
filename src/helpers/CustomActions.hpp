#ifndef __osu_callfunc_percentage__
#define __osu_callfunc_percentage__

#include <Geode/Geode.hpp>

using namespace geode::prelude;

typedef void (CCObject::*CallFuncP)(float);
#define callfuncp_selector(...) (CallFuncP)(&__VA_ARGS__)

class osuCallFuncP : public CCActionInterval {
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
    static osuCallFuncP* create(float from, float to, float duration, CCObject* target, CallFuncP selector);
    osuCallFuncP* reverse();
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

#define CCEaseOutQuad(action) CCEaseOut::create(action, 4)
#define CCEaseOutQuint(action) CCEaseOut::create(action, 5)

#endif