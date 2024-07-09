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

#endif