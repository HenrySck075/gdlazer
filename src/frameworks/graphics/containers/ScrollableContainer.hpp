#pragma once

#include "Container.hpp"
#include <Geode/Geode.hpp>

GDL_NS_START

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
    void addChild(CCNode* child) override;
    void addChild(CCNode* child, int zOrder) override;
    void addChild(CCNode* child, int zOrder, int tag) override;
    
    void onMouseDragMove(MouseDragEvent* event) override;
    void onMouseDragStop(MouseDragEvent* event) override;
    void onMouseDragStart(MouseDragEvent* event) override;

    enum class ScrollDirection {
        None,
        Vertical,
        Horizontal,
        Both
    };

    void setScrollVelocity(float velocity) { m_scrollVelocity = velocity; }
    float getScrollVelocity() const { return m_scrollVelocity; }
    
    void setScrollDirection(ScrollDirection direction) { m_scrollDirection = direction; }
    ScrollDirection getScrollDirection() const { return m_scrollDirection; }
    
protected:
    void updateSizeWithUnit() override;
    void updateChildPosition();
    void applyInertia(float dt);

private:
    CCNode* m_content = nullptr;
    cocos2d::CCPoint m_scrollPosition = CCPointZero;
    cocos2d::CCPoint m_scrollVelocityVec = CCPointZero;
    float m_scrollVelocity = 1.0f;
    ScrollDirection m_scrollDirection = ScrollDirection::Vertical;
};

GDL_NS_END
