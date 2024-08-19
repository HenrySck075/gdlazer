#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../../utils.hpp"
using namespace cocos2d;

// Scale9Sprite without the 9 sprites part
class CCResizableSprite : public CCNode {
private:
  CCSize baseSize = CCSize(0,0);
  CCSprite* m_sprite;
public:
  static CCResizableSprite* create(); 
  static CCResizableSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
  static CCResizableSprite* createWithSpriteFrameName(const char* frameName); 
  bool initWithSpriteFrameName(const char* frameName);
  void setContentSize(CCSize const& size) override;
};
