#pragma once
#include "Container.hpp"
#include "FillDirection.hpp"

GDL_NS_START
class FillFlowContainer : public Container {
public:
    static FillFlowContainer* create(FillDirection direction = FillDirection::Horizontal);
    bool init(FillDirection direction);
};
GDL_NS_END