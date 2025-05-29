#include "CustomActions.hpp"

bool CCCustomTween::initWithACatgirl(float from, float to, float duration, CCObject* target, CallFuncP selector) {
  if (!CCActionInterval::initWithDuration(duration)) {
    return false;
  };

  m_from = from;
  m_to = to;
  m_delta = to-from;
  m_target = target;
  m_pCallFunc = selector;
  geode::log::debug("[CCCustomTween]: {} f: {} t: {}", m_delta, m_from, m_to);
  return true;
}

void CCCustomTween::update(float time) {
  // distance * current time + start value
  float v = m_delta * time + m_from;
  execute(v);
}

void CCCustomTween::execute(float value) {
  (m_target->*m_pCallFunc)(value);
}

CCCustomTween* CCCustomTween::create(float from, float to, float duration, CCObject* target, CallFuncP selector) {
  $createClass(CCCustomTween, initWithACatgirl, from, to, duration, target, selector);
}

CCCustomTween* CCCustomTween::reverse() {
  return CCCustomTween::create(m_to, m_from, m_fDuration, m_target, m_pCallFunc);
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
  if (initWithAction(action) && initWithDuration(action->getDuration() - startTime)) {
    m_fInnerDuration = action->getDuration();
    m_fStartTime = startTime;
    action->retain();
    return true;
  }
  return false;
}

/*
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
*/
CCActionSkip::~CCActionSkip() {
  CC_SAFE_RELEASE(m_pInner);
}

void CCActionSkip::update(float time) {
  m_pInner->update((m_fDuration*time + m_fStartTime) / m_fInnerDuration);
}

//
// SizeTo
//
CCResizeTo* CCResizeTo::create(float duration, float s)
{
  CCResizeTo* pSizeTo = new CCResizeTo();
  pSizeTo->initWithDuration(duration, s);
  pSizeTo->autorelease();

  return pSizeTo;
}

bool CCResizeTo::initWithDuration(float duration, float s)
{
  if (CCActionInterval::initWithDuration(duration))
  {
    m_fEndContentWidth = s;
    m_fEndContentHeight = s;

    return true;
  }

  return false;
}

CCResizeTo* CCResizeTo::create(float duration, float sx, float sy)
{
  CCResizeTo* pSizeTo = new CCResizeTo();
  pSizeTo->initWithDuration(duration, sx, sy);
  pSizeTo->autorelease();

  return pSizeTo;
}

bool CCResizeTo::initWithDuration(float duration, float sx, float sy)
{
  if (CCActionInterval::initWithDuration(duration))
  {
    m_fEndContentWidth = sx;
    m_fEndContentHeight = sy;

    return true;
  }

  return false;
}
/*
CCObject* CCResizeTo::copyWithZone(CCZone* pZone)
{
  CCZone* pNewZone = NULL;
  CCResizeTo* pCopy = NULL;
  if (pZone && pZone->m_pCopyObject)
  {
    //in case of being called at sub class
    pCopy = (CCResizeTo*)(pZone->m_pCopyObject);
  }
  else
  {
    pCopy = new CCResizeTo();
    pZone = pNewZone = new CCZone(pCopy);
  }

  CCActionInterval::copyWithZone(pZone);


  pCopy->initWithDuration(m_fDuration, m_fEndContentWidth, m_fEndContentHeight);

  CC_SAFE_DELETE(pNewZone);
  return pCopy;
}
*/
void CCResizeTo::startWithTarget(CCNode* pTarget) {
  CCActionInterval::startWithTarget(pTarget);
  m_fStartContentWidth = pTarget->getContentWidth();
  m_fStartContentHeight = pTarget->getContentHeight();
  m_fDeltaWidth = m_fEndContentWidth - m_fStartContentWidth;
  m_fDeltaHeight = m_fEndContentHeight - m_fStartContentHeight;
}

void CCResizeTo::update(float time)
{
  if (m_pTarget) {
    m_pTarget->setContentSize({
      m_fStartContentWidth + m_fDeltaWidth * time,
      m_fStartContentHeight + m_fDeltaHeight * time
    });
  }
}


//
// CallFuncL
//
CCCallFuncL * CCCallFuncL::create(SEL_CallFuncL selector) 
{
  CCCallFuncL *pRet = new CCCallFuncL();

  if (pRet) {
    pRet->m_pCallFunc = selector;
    pRet->autorelease();
    return pRet;
  }

  CC_SAFE_DELETE(pRet);
  return NULL;
}

CCCallFuncL::~CCCallFuncL(void)
{
  if (m_nScriptHandler)
  {
    cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nScriptHandler);
  }
}
/*
CCObject * CCCallFuncL::copyWithZone(CCZone *pZone) {
  CCZone* pNewZone = NULL;
  CCCallFuncL* pRet = NULL;

  if (pZone && pZone->m_pCopyObject) {
    //in case of being called at sub class
    pRet = (CCCallFuncL*) (pZone->m_pCopyObject);
  } else {
    pRet = new CCCallFuncL();
    pZone = pNewZone = new CCZone(pRet);
  }

  CCActionInstant::copyWithZone(pZone);
  pRet->m_pCallFunc = m_pCallFunc;
  if (m_nScriptHandler > 0 ) {
    pRet->m_nScriptHandler = cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine()->reallocateScriptHandler(m_nScriptHandler);
  }
  CC_SAFE_DELETE(pNewZone);
  return pRet;
}
*/
void CCCallFuncL::update(float time) {
  CC_UNUSED_PARAM(time);
  m_pCallFunc();
}
