#pragma once
/// Unorganized collections of utilities
#include <Geode/Geode.hpp>
#include <cmath>
#include <cstddef>
#include "macro.h"
using namespace geode::prelude;
#undef interface // whoever does this should till yhemselves
float randomFloat();
bool randomBool();

#define rgbColor(r,g,b) r/255.f, g/255.f, b/255.f

/// Assured reimplementation of CCNode::boundingBox()
/// <param name="node">| the node to get</param>
/// <returns>the node's bounding box in the top most parent in the node's parent tree</returns>
CCRect boundingBoxFromContentSize(CCNode* node);
CCPoint convertToNodeSpaceA(CCNode* node, CCPoint point);
// this mf uses ypos the other way
CCRect flipRect(CCRect rect);

#define degreeToRadius(deg) (deg) / 180 * M_PI


// use wisely
template<typename interface, typename obj>
inline bool didImplements(obj object) {
  return typeinfo_cast<interface*>(object) != nullptr;
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
  if (didImplements<ParentType>(parent)) {
    return static_cast<ParentType*>(parent);
  };
  parent = parent->getParent();
  }
  return nullptr;
}


float uiWidthToGL(float length);
float uiHeightToGL(float length);

template<class T>
T* postconfig(T* node, std::function<void(T*)> func) {
  func(node);
  return node;
}

//uintptr_t getGeodeLib();

std::string getObjectName(CCObject* node);

void setWindowTitleReal(std::string title);

CCNode* getChildByIdTree(
  CCNode *node, std::initializer_list<std::variant<std::string, int>> path
);