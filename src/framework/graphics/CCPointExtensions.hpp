///@note Equivalent file name is Vector2Extensions
#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include <cmath>
using namespace cocos2d;

class CCPointExtensions {
  public:
  static float distance(CCPoint const& point1, CCPoint const& point2) {
    return sqrtf((point2.x - point1.x) * (point2.x - point1.x) + (point2.y - point1.y) * (point2.y - point1.y));
  }
};