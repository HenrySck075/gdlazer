#pragma once
#include "Container.hpp"
#include "FillDirection.hpp"

GDF_NS_START
/// A Container that positions its children along an axis
class FillFlowContainer : public Container {
private:
    $kwargsStruct(
        FillDirection, direction, FillDirection::Horizontal,
        geode::Anchor, anchor, Anchor::BottomLeft,
        float, gap, 0,
        bool, axisReverse, false,
        bool, autoResize, true
    )
public:
    static FillFlowContainer* create(GDF_KWARGS);
    bool init(GDF_KWARGS);
    void setChildAnchor(geode::Anchor anchor);
    void setGap(float gap);
    float getGap();
    void setAxisReverse(bool axisReverse);
    void setAutoResize(bool enable);
    bool getAutoResize() const { return m_autoResize; }
    bool getAxisReverse() const { return m_axisReverse; }
    FillDirection getDirection() const { return m_direction; }
    geode::Anchor getChildAnchor() const { return m_anchor; }
    void updateLayout();
};
GDF_NS_END
