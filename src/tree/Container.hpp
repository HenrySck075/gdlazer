#pragma once
#include "RectangleF.hpp"
#include "col/Color4.hpp"
#include "shared_ptr_2.hpp"
#include "types/Vector2.hpp"

struct Container;
using ContainerPtr = shared_ptr_ctor<Container>;

struct Container {
    Vector2 size;
    RectangleF padding;
    Color4 backgroundColor;
    std::vector<ContainerPtr> children;
    std::string id;
    std::string classes;
};


class ElementBase {
  virtual cocos2d::CCNode* build();
}; 


/// implement the following methods:
/// - cocos2d::CCNode* build();
template<class T>
class Element;

#define $defineElement(cls) \
template<> \
class Element<cls> : public ElementBase
