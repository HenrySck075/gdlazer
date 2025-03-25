#pragma once

#include "Container.hpp"
#include "Geode/cocos/base_nodes/CCNode.h"
#include <Geode/Geode.hpp>

GDF_NS_START

class ScrollableContainer : public Container {
public:
    static ScrollableContainer* create() {
        auto ret = new ScrollableContainer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() override;
    // it only allows 1 child for now 
    // but maybe i'll implicitly wrap these in a container 
    // in the future
    void addChild(cocos2d::CCNode* child) override;
    void addChild(cocos2d::CCNode* child, int zOrder) override;
    void addChild(cocos2d::CCNode* child, int zOrder, int tag) override;

    void resizeToChildSize();
    
    enum class ScrollDirection {
        None,
        Vertical,
        Horizontal,
        Both
    };

    void setScrollVelocity(float velocity) { m_scrollVelocity = velocity; }
    float getScrollVelocity() const { return m_scrollVelocity; }

    /// im not aware of any function that can return the nearly exact value  i have to guess it
    float guessVelocityFromDistance(float dist) {
      float ret = dist < 0 ? -0.1f : 0.1f;
      float movedDist = ret;
      while (abs(movedDist) < abs(dist)) {
        ret = ret/m_velocityFriction;
        movedDist+=ret;
      }
      ret = dist/movedDist*ret;
      return ret;
    }
    
    void setScrollDirection(ScrollDirection direction) { m_scrollDirection = direction; }
    ScrollDirection getScrollDirection() const { return m_scrollDirection; }
    
protected:
    void updateSizeWithUnit() override;
    void updateChildPosition();
    void applyInertia(float dt);

private:
    CCNode* m_content = nullptr;
    cocos2d::CCPoint m_scrollPosition = {0,0};
    cocos2d::CCPoint m_scrollVelocityVec = {0,0};
    float m_scrollVelocity = 1.0f;
    float m_velocityFriction = 0.95f;
    bool m_returningInBounds = false;
    ScrollDirection m_scrollDirection = ScrollDirection::Vertical;
};

GDF_NS_END
