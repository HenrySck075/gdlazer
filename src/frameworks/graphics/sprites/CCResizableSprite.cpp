#include "CCResizableSprite.hpp"
#include "../../../utils.hpp"
GDF_NS_START
CCResizableSprite* CCResizableSprite::create() {
  $createClass(CCResizableSprite, init);
}

CCResizableSprite* CCResizableSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
  $createClass(CCResizableSprite, initWithSpriteFrameName, pszSpriteFrameName);
}
CCResizableSprite* CCResizableSprite::createWithTexture(CCTexture2D* texture) {
  $createClass(CCResizableSprite, initWithTexture, texture);
};

bool CCResizableSprite::init() {
  CCNode::init();
  m_sprite = CCSprite::create();
  return finishInit();
}

bool CCResizableSprite::initWithTexture(CCTexture2D* texture) {
  CCNode::init();
  m_sprite = CCSprite::createWithTexture(texture);
  baseSize = m_sprite->getContentSize();
  m_sprite->setPosition(baseSize/2);
  return finishInit();
}

bool CCResizableSprite::initWithSpriteFrameName(const char* pszSpriteFrameName) {
  CCNodeRGBA::init();
  m_sprite = CCSprite::createWithSpriteFrameName(pszSpriteFrameName);
  baseSize = m_sprite->getContentSize();
  m_sprite->setPosition(baseSize/2);
  return finishInit();
}

bool CCResizableSprite::finishInit() {
  addChild(m_sprite);
  //setCascadeOpacityEnabled(true);
  setCascadeColorEnabled(true);
  setAnchorPoint({0.5,0.5});
  //setOpacity()
  return true;
}

void CCResizableSprite::setContentSize(CCSize const& size) {
  CCNode::setContentSize(size);
  refreshScaling();
}

void CCResizableSprite::refreshScaling() {
  if (baseSize.equals(CCSize{0,0})) {
    baseSize = m_sprite->getContentSize();
  }
  if (baseSize.equals(CCSize{0,0})) return;

  auto size = CCNode::getContentSize();
  float wRatio = size.width / baseSize.width;
  float hRatio = size.height / baseSize.height;
  switch (m_boxFit) {
    case BoxFit::Fill:
      break;
    case BoxFit::Contain:
      if (wRatio < hRatio) {hRatio = wRatio;}
      else wRatio = hRatio;
      break;
    case BoxFit::Cover:
      if (wRatio > hRatio) {hRatio = wRatio;}
      else wRatio = hRatio;
      break;
    case BoxFit::FitWidth:
      hRatio = wRatio;
      break;
    case BoxFit::FitHeight:
      wRatio = hRatio;
      break;
    default:
      break;
  }
  
  m_sprite->setScaleX(wRatio*m_wsscale);
  m_sprite->setScaleY(hRatio*m_wsscale);
  m_sprite->setPosition(size/2);

}
GDF_NS_END