#pragma once
#include "Container.hpp"
#include "FillDirection.hpp"

class FillFlowContainer : public Container {
public:
    static FillFlowContainer* create(FillDirection direction = FillDirection::Horizontal);
    bool init(FillDirection direction);
};
