#include <Geode/cocos/include/cocos2d.h>
#include "../../utils.hpp"
using namespace cocos2d;

class CCResizableSprite : public CCSprite {
private:
  CCSize baseSize = CCSize(0,0);
public:
  static CCResizableSprite* create(); 
  static CCResizableSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
  static CCResizableSprite* createWithSpriteFrameName(const char* frameName); 
  void setContentSize(CCSize& size);
};