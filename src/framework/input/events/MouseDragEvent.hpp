#pragma once

#include <Geode/cocos/include/cocos2d.h>

class MouseDragEvent{
public:
    CCPoint const& startPos;
    CCPoint current;
    MouseDragEvent(decltype(startPos) start, decltype(current) cur) : startPos(start), current(cur) {}
};