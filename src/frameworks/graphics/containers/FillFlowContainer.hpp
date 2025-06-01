#pragma once
#include "Container.hpp"
#include "FillDirection.hpp"
#include "FillFlowLayout.hpp"

GDF_NS_START
/// A Container that positions its children along an axis
class FillFlowContainer : public Container {
    FillFlowLayout* m_layout;
public:
    static FillFlowContainer* create(FillDirection direction = FillDirection::Horizontal);
    bool init(FillDirection direction);
    void setGap(float gap);
    float getGap();
};
GDF_NS_END
