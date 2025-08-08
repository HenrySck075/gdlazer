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

template<class T>
struct Element {
    Container widget; // bro why dont templating makes this visible
    Element<T>(T& elem) : widget(std::move(elem)) {};
    cocos2d::CCNode* build();
    bool amogus;
};

template<>
struct Element<Container> {
    cocos2d::CCNode* build();
};