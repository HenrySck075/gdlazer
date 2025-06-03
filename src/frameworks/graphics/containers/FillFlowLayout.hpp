#pragma once
#include <Geode/ui/Layout.hpp>
#include "FillDirection.hpp"

GDF_NS_START
class FillFlowLayout : public geode::AxisLayout {
protected:
    FillDirection m_direction;
    geode::Anchor m_anchor;
public:
    FillFlowLayout(FillDirection direction, geode::Anchor anchor)
     : geode::AxisLayout((geode::Axis)direction), m_direction(direction), m_anchor(anchor) {}
    static FillFlowLayout* create(FillDirection direction = FillDirection::Horizontal, geode::Anchor anchor = geode::Anchor::BottomLeft);
    void apply(cocos2d::CCNode* on) override;

    void setAnchor(geode::Anchor anchor) {m_anchor = anchor;}
};
GDF_NS_END
