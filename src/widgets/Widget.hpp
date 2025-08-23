#pragma once
#include "shared_ptr_2.hpp"

struct Widget;
using WidgetPtr = shared_ptr_ctor<Widget>;

struct Widget {
    std::string id;
    std::string classes;
};
