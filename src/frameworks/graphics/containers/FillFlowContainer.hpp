#pragma once
#include "Container.hpp"
#include "FillDirection.hpp"
#include "FillFlowLayout.hpp"

GDF_NS_START
/// A Container that positions its children along an axis
class FillFlowContainer : public Container {
    FillFlowLayout* m_layout;
public:
    static FillFlowContainer* create(FillDirection direction = FillDirection::Horizontal, geode::Anchor anchor = Anchor::BottomLeft);
    bool init(FillDirection direction, geode::Anchor anchor);
    void setChildAnchor(geode::Anchor anchor);
    void setGap(float gap);
    float getGap();
    void setAxisReverse(bool axisReverse);
    void setAutoGrowAxis(bool enable);
};
GDF_NS_END
