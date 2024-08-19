#include "CCResizableSprite.hpp"

CCResizableSprite* CCResizableSprite::create() {
  create_class(CCResizableSprite, init);
}

CCResizableSprite* CCResizableSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    create_class(CCResizableSprite, initWithSpriteFrameName, pszSpriteFrameName);
}

bool CCResizableSprite::initWithSpriteFrameName(const char* pszSpriteFrameName) {
    CCNode::init();
    m_sprite = CCSprite::createWithSpriteFrameName(pszSpriteFrameName);
    baseSize = m_sprite->getContentSize();
    m_sprite->setPosition(baseSize/2);
    addChild(m_sprite);
    setAnchorPoint(ccp(0.5,0.5));
    return true;
}

void CCResizableSprite::setContentSize(CCSize const& size) {
    CCNode::setContentSize(size);
    if (baseSize.equals(CCSize(0,0))) {
        baseSize = getContentSize();
    }
    float wRatio = size.width / baseSize.width;
    float hRatio = size.height / baseSize.height;
    
    m_sprite->setScaleX(wRatio);
    m_sprite->setScaleY(hRatio);
    m_sprite->setPosition(size/2);
}
