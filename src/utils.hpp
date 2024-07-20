#pragma once
// file in date ordee
#include "Geode/Geode.hpp"
#include "Geode/cocos/platform/CCPlatformMacros.h"
#include <cmath>
#include <cstddef>
using namespace geode::prelude;
#undef interface // whoever does this should till yhemselves
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
      ret->CCObject::autorelease(); \
    } else { \
      delete ret; \
    } \
\
    return ret

#define degreeToRadius(deg) deg/360*M_PI

#define glPointWidth(w) w/1366
#define glPointHeight(w) w/768
#define glPoint(w,h) glPointWidth(w), glPointHeight(h)

// use wisely
template<typename interface, typename obj>
bool didImplements(obj object) {
  return dynamic_cast<interface*>(object) != nullptr;
}


std::string& trim( std::string& s, char c, bool reverse = false );

/*
template<typename TargetType, typename ComparingType>
bool isType(ComparingType obj);
*/
template<class ParentType = CCNode>
ParentType* getParentOfType(CCNode* node){
  CCNode* parent = node->getParent();
  while (parent!=nullptr) {
    if (std::is_same_v<ParentType*,decltype(parent)>) {
      return static_cast<ParentType*>(parent);
    };
    parent = parent->getParent();
  }
  return nullptr;
}
