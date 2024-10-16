#pragma once

#include <Geode/cocos/include/cocos2d.h>

class MouseDragEvent : public cocos2d::CCObject {
public:
    CCPoint startPos;
    CCPoint current;
    MouseDragEvent(CCPoint start, CCPoint cur) : startPos(start), current(cur) {
    }
};