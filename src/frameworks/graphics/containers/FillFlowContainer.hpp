#pragma once
#include "Container.hpp"
#include "FillDirection.hpp"

GDF_NS_START
/// A Container that positions its children along an axis
class FillFlowContainer : public Container {
public:
    static FillFlowContainer* create(FillDirection direction = FillDirection::Horizontal);
    bool init(FillDirection direction);
    void setGap(float gap);
    float getGap();
};
GDF_NS_END
