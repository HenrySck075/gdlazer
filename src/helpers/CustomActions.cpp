#include "CustomActions.hpp"

bool osuCallFuncP::initWithACatgirl(float from, float to, float duration, CCObject* target, CallFuncP selector) {
    if (!CCActionInterval::initWithDuration(duration)) {
        return false;
    };

    m_reversed = from>to;
    if (!m_reversed) {
        m_from = from;
        m_to = to;
    }
    else {
        m_from = to;
        m_to = from;
    }
    m_target = target;
    return true;
}

void osuCallFuncP::update(float time) {
    // percentage distance * current time percentage + percent start value
    this->execute((m_to-m_from)*(m_reversed?1-(time/m_fDuration):time/m_fDuration)+m_from);
}

void osuCallFuncP::execute(float percentage) {
    (m_target->*m_pCallFunc)(percentage);
}

osuCallFuncP* osuCallFuncP::create(float from, float to, float duration, CCObject* target, CallFuncP selector) {
    osuCallFuncP* pRet = new osuCallFuncP();
    if (pRet && pRet->initWithACatgirl(from, to, duration, target, selector)) {
        pRet->m_pCallFunc = selector;
        return pRet;
    }
    else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

osuCallFuncP* osuCallFuncP::reverse() {
    return osuCallFuncP::create(m_to, m_from, m_fDuration, m_target, m_pCallFunc);
}

/// CCActionSkip

CCActionSkip* CCActionSkip::create(float startTime, CCActionInterval* action) {
    CCActionSkip* pRet = new CCActionSkip();
    if (pRet && pRet->initWithACatgirl(action, startTime)) {
        return pRet;
    }
    else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCActionSkip::initWithACatgirl(CCActionInterval* action, float startTime) {
    if (this->initWithAction(action) && this->initWithDuration(action->getDuration() - startTime)) {
        this->m_fStartTime = startTime;
        action->retain();
        return true;
    }
    return false;
}


CCObject* CCActionSkip::copyWithZone(CCZone* pZone)
{
    CCZone* pNewZone = NULL;
    CCActionSkip* pCopy = NULL;
    if (pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCActionSkip*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCActionSkip();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithAction((CCActionInterval*)(m_pInner->copy()->autorelease()));
    pCopy->initWithDuration(m_pInner->getDuration() - m_fStartTime);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCActionSkip::~CCActionSkip() {
    CC_SAFE_RELEASE(m_pInner);
}

// why is this delays the action
void CCActionSkip::update(float time) {
    m_pInner->update(time + 5.f);
}