#pragma once

#include "Container.hpp"
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/Geode.hpp>

GDF_NS_START

enum class ScrollDirection {
    None,
    Vertical,
    Horizontal,
    Both
};

class ScrollContainer : public Container {
public:
    static ScrollContainer* create(Container* content, ScrollDirection direction = ScrollDirection::Vertical, AxisAlignment crossAxisAlignment = AxisAlignment::Start) {
        $createClass(ScrollContainer, init, content, direction, crossAxisAlignment);
    }

    bool init(Container* content, ScrollDirection direction = ScrollDirection::Vertical, AxisAlignment crossAxisAlignment = AxisAlignment::Start);
    void setContent(Container* child);

    void resizeToChildSize();
    
    void scrollBy(cocos2d::CCPoint const& dist, bool animate = false);
    void scrollTo(cocos2d::CCPoint const& position, bool animate = false);

    void setScrollVelocity(float velocity) { m_scrollVelocity = velocity; }
    float getScrollVelocity() const { return m_scrollVelocity; }

    float guessVelocityFromDistance(float dist) {
        float ret = dist < 0 ? -0.1f : 0.1f;
        float movedDist = ret;
        while (abs(movedDist) < abs(dist)) {
            ret = ret/m_velocityFriction;
            movedDist += ret;
        }
        ret = dist/movedDist * ret;
        return ret;
    }

    void setScrollDirection(ScrollDirection direction);
    ScrollDirection getScrollDirection() const { return m_scrollDirection; }
    void setCrossAxisAlignment(AxisAlignment alignment);
    AxisAlignment getCrossAxisAlignment() const { return m_crossAxisAlignment; }

protected:
    void updateSize() override;
    void updatePosition();
    void applyInertia(float dt);

private:
    bool m_vfuncCallLoopBlock = false;
    Container* m_content = nullptr;
    cocos2d::CCPoint m_scrollPosition = {0,0};
    cocos2d::CCPoint m_scrollVelocityVec = {0,0};
    float m_scrollVelocity = 1.0f;
    float m_velocityFriction = 0.95f;
    bool m_returningInBounds = false;
    ScrollDirection m_scrollDirection = ScrollDirection::Vertical;
    AxisAlignment m_crossAxisAlignment = AxisAlignment::Start;
};

GDF_NS_END
