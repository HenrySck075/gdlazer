#pragma once
#include "Container.hpp"
#include "../../bindables/Bindable.hpp"
#include "FillDirection.hpp"

GDF_NS_START
/// A Container that positions its children along an axis
class FillFlowContainer : public Container {
private:
    FillDirection m_direction;
    geode::Anchor m_anchor;
    float m_gap = 0;
    bool m_axisReverse = false;
    bool m_autoResize = true;

    EventListener<NodeLayoutUpdated> m_autoUpdateListener;
    Bindable<bool> m_autoUpdate = true;

public:
    static FillFlowContainer* create(FillDirection direction = FillDirection::Horizontal, geode::Anchor anchor = Anchor::BottomLeft);
    bool init(FillDirection direction, geode::Anchor anchor);
    void setChildAnchor(geode::Anchor anchor);
    void setGap(float gap);
    float getGap();
    void setAxisReverse(bool axisReverse);
    void setAutoResize(bool enable);
    bool getAutoResize() const { return m_autoResize; }
    bool getAxisReverse() const { return m_axisReverse; }
    FillDirection getDirection() const { return m_direction; }
    geode::Anchor getChildAnchor() const { return m_anchor; }
    /// Let the Container automatically calls `updateLayout` on NodeLayoutUpdated
    void setAutoUpdateLayout(bool enable);

    void updateLayout();
};
GDF_NS_END
