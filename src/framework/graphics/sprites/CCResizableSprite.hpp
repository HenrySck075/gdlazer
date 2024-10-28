#pragma once

#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

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

  void refreshScaling(); 

  void setBoxFit(BoxFit fit) {
    m_boxFit = fit;
    refreshScaling();
  }

  void setTextureRect(CCRect const& rect) {
    m_sprite->setTextureRect(rect);
    baseSize = rect.size;
    if (getContentSize()==CCSize{0,0}) CCNode::setContentSize(baseSize);
    refreshScaling();
  }

  CCSprite* getSprite(){return m_sprite;}
  void setSprite(CCSprite* spr){m_sprite = spr;}
};
