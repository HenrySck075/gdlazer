#include "CustomActions.hpp"

bool CCCallFuncP::initWithACatgirl(float from, float to, float duration, CCObject* target, CallFuncP selector) {
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

void CCCallFuncP::update(float time) {
  // percentage distance * current time percentage + percent start value
  this->execute((m_to-m_from)*(m_reversed?1-(time/m_fDuration):time/m_fDuration)+m_from);
}

void CCCallFuncP::execute(float percentage) {
  (m_target->*m_pCallFunc)(percentage);
}

CCCallFuncP* CCCallFuncP::create(float from, float to, float duration, CCObject* target, CallFuncP selector) {
  CCCallFuncP* pRet = new CCCallFuncP();
  if (pRet && pRet->initWithACatgirl(from, to, duration, target, selector)) {
    pRet->m_pCallFunc = selector;
    return pRet;
  }
  else {
    CC_SAFE_DELETE(pRet);
  }
  return pRet;
}

CCCallFuncP* CCCallFuncP::reverse() {
  return CCCallFuncP::create(m_to, m_from, m_fDuration, m_target, m_pCallFunc);
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
    m_fInnerDuration = action->getDuration();
    this->m_fStartTime = startTime / action->getDuration();
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

void CCActionSkip::update(float time) {
  m_pInner->update(time + (m_fStartTime/m_fInnerDuration));
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

void CCResizeTo::startWithTarget(CCNode* pTarget)
{
  CCActionInterval::startWithTarget(pTarget);
  m_fStartContentWidth = pTarget->getContentWidth();
  m_fStartContentHeight = pTarget->getContentHeight();
  m_fDeltaX = m_fEndContentWidth - m_fStartContentWidth;
  m_fDeltaY = m_fEndContentHeight - m_fStartContentHeight;
}

void CCResizeTo::update(float time)
{
  if (m_pTarget)
  {
    m_pTarget->setContentWidth(m_fStartContentWidth + m_fDeltaX * time);
    m_pTarget->setContentHeight(m_fStartContentHeight + m_fDeltaY * time);
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

void CCCallFuncL::update(float time) {
  CC_UNUSED_PARAM(time);
  m_pCallFunc();
}
