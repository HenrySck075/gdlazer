#ifndef __osulazer__utils__
#define __osulazer__utils__
// file in date ordee
#include "Geode/Geode.hpp"
#include "Geode/cocos/platform/CCPlatformMacros.h"
using namespace geode::prelude;

float randomFloat();
bool randomBool();

#define rgbColor(r,g,b) r/255.f, g/255.f, b/255.f

/// <summary>
/// Get a node's bounding box from its content size rather than whatever cocos2dx decides to return
/// </summary>
/// <param name="node">| the node to get</param>
/// <returns>the node's bounding box in the top most parent in the node's parent tree</returns>
CCRect boundingBoxFromContentSize(CCNode* node);
// this mf uses ypos the other way
CCRect flipRect(CCRect rect);

#define create_class(classname, initfunc, ...) \
    classname* ret = new classname(); \
    if (ret && ret->initfunc(__VA_ARGS__)) { \
      ret->autorelease(); \
    } else { \
      CC_SAFE_DELETE(ret); \
    } \
\
    return ret


#endif
