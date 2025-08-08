#pragma once
#include "RectangleF.hpp"
#include "col/Color4.hpp"
#include "tree/shared_ptr_2.hpp"

struct Container;
using ContainerPtr = shared_ptr_ctor<Container>;

struct Container {
    RectangleF rect;
    RectangleF padding;
    Color4 backgroundColor;
    std::vector<ContainerPtr> children;
    std::string id;
    std::string classes;
};

template<class T>
struct DrawHandler {};