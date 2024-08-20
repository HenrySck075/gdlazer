#pragma once

#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

// Scale9Sprite without the 9 sprites part
class CCResizableSprite : public CCNode {
private:
  CCSize baseSize = CCSize(0,0);
  CCSprite* m_sprite;
  bool m_preserveRatio = false;
public:
  static CCResizableSprite* create(); 
  static CCResizableSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
  static CCResizableSprite* createWithSpriteFrameName(const char* frameName); 
  bool initWithSpriteFrameName(const char* frameName);
  bool init();
  void setContentSize(CCSize const& size) override;

  void preserveRatio(bool enable) {m_preserveRatio = enable;}
  CCSprite* getSprite(){return m_sprite;}
};
