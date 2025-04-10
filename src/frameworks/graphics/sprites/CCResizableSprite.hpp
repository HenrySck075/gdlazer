#pragma once

#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

#include "../../../macro.h"

GDF_NS_START
enum class BoxFit {
  Fill,
  Contain,
  Cover,
  FitWidth,
  FitHeight
};

// Scale9Sprite without the 9 sprites part
class CCResizableSprite : public CCNodeRGBA {
private:
  CCSize baseSize = CCSize(0,0);
  float m_wsscale = 1;
  CCSprite* m_sprite;
  BoxFit m_boxFit = BoxFit::Fill;
  bool finishInit();
public:
  static CCResizableSprite* create(); 
  static CCResizableSprite* createWithTexture(CCTexture2D* texture);
  static CCResizableSprite* createWithSpriteFrameName(const char* frameName);
  bool initWithSpriteFrameName(const char* frameName);
  bool initWithTexture(CCTexture2D* texture);
  bool init();
  void setContentSize(CCSize const& size) override;
  void setWrappedSpriteScale(float scale) {
    m_wsscale = scale;
  }

  void refreshScaling(); 

  void setBoxFit(BoxFit fit) {
    m_boxFit = fit;
    refreshScaling();
  }

  void setTextureRect(CCRect const& rect) {
    m_sprite->setTextureRect(rect);
    baseSize = rect.size;
    if (getContentSize().equals({0,0})) CCNode::setContentSize(baseSize);
    refreshScaling();
  }

  void setOpacity(GLubyte opacity) override {
    m_sprite->setOpacity(opacity);
    CCNodeRGBA::setOpacity(opacity);
  }

  CCSprite* getSprite(){return m_sprite;}
  void setSprite(CCSprite* spr){m_sprite = spr;}
};
GDF_NS_END