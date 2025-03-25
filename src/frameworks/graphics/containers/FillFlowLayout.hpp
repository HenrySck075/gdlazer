#pragma once
#include <Geode/ui/Layout.hpp>
#include "FillDirection.hpp"

GDF_NS_START
class FillFlowLayout : public geode::AxisLayout {
protected:
    FillDirection m_direction;

public:
    FillFlowLayout(FillDirection direction) : geode::AxisLayout((geode::Axis)direction), m_direction(FillDirection::Horizontal) {}
    static FillFlowLayout* create(FillDirection direction = FillDirection::Horizontal);
    void apply(cocos2d::CCNode* on) override;
};
GDF_NS_END
