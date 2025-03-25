#pragma once
#include "../../../macro.h"
#include "Geode/cocos/actions/CCActionInterval.h"

GDF_NS_START

namespace animations {
  class TintTo : public cocos2d::CCActionInterval {
    cocos2d::ccColor4B m_destColor;
    cocos2d::ccColor4B m_startColor;
  public:
    bool initWithColor(float d, cocos2d::ccColor4B c);
    static TintTo *create(float duration, cocos2d::ccColor4B color);
    void update(float dt) override;
    void startWithTarget(cocos2d::CCNode* target) override;
  };
};

GDF_NS_END
