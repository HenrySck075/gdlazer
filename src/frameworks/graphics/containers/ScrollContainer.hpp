#pragma once

#include "Container.hpp"
#include "Geode/cocos/base_nodes/CCNode.h"
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
  static ScrollContainer* create(Container* content) {
    $createClass(ScrollContainer, init, content);
  }

  bool init(Container* content);
  void setContent(cocos2d::CCNode* child);

  void resizeToChildSize();

  /// Scrolls by a specified distance.
  ///
  /// An axis will be ignored if it is locked from scrolling.
  void scrollBy(cocos2d::CCPoint const& dist, bool animate = false);

  void scrollTo(cocos2d::CCPoint const& position, bool animate = false);

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
  void updateSize() override;
  void updatePosition();
  void applyInertia(float dt);

private:
  /// wtf!!!!
  bool m_vfuncCallLoopBlock = false;
  CCNode* m_content = nullptr;
  cocos2d::CCPoint m_scrollPosition = {0,0};
  cocos2d::CCPoint m_scrollVelocityVec = {0,0};
  float m_scrollVelocity = 1.0f;
  float m_velocityFriction = 0.95f;
  bool m_returningInBounds = false;
  ScrollDirection m_scrollDirection = ScrollDirection::Vertical;
};

/// A scrollbar for a ScrollContainer
class ScrollbarContainer : public Container {
public:
  static ScrollbarContainer* create(ScrollContainer* scrollContainer) {
    auto ret = new ScrollbarContainer();
    if (ret && ret->init(scrollContainer)) {
      ret->autorelease();
      return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
  }

  bool init(ScrollContainer* scrollContainer);
};

GDF_NS_END
