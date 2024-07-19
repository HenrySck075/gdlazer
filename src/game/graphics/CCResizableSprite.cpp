#include "CCResizableSprite.hpp"

CCResizableSprite* CCResizableSprite::create() {
  create_class(CCResizableSprite, init);
}

CCResizableSprite* CCResizableSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    CCResizableSprite *pobSprite = new CCResizableSprite();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CCResizableSprite* CCResizableSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pFrame != NULL, msg);
#endif
    
    return createWithSpriteFrame(pFrame);
}

void CCResizableSprite::setContentSize(CCSize& size) {
  if (baseSize.equals(CCSize(0,0))) {
    baseSize = getContentSize();
  }
  float wRatio = size.width / baseSize.width;
  float hRatio = size.height / baseSize.height;
  
  CCSprite::setScaleX(wRatio);
  CCSprite::setScaleY(hRatio);
}
