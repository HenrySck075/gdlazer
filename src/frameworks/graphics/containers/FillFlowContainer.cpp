#include "FillFlowContainer.hpp"

GDF_NS_START

FillFlowContainer* FillFlowContainer::create(GDF_KWARGS) {
    $createClass2(FillFlowContainer, init);
}
bool FillFlowContainer::init(GDF_KWARGS) {
    if (!Container::init()) return false;
    
    ctorInitArgs(args);
    
    return true;
}
void FillFlowContainer::setGap(float gap) {
    m_gap = gap;
}
float FillFlowContainer::getGap() {
    return m_gap;
}
void FillFlowContainer::setAxisReverse(bool axisReverse) {
    m_axisReverse = axisReverse;
}
void FillFlowContainer::setAutoResize(bool enable) {
    m_autoResize = enable;
};
void FillFlowContainer::setChildAnchor(geode::Anchor anchor) {
    m_anchor = anchor;
}

CCPoint getAnchorPointFromAnchor(geode::Anchor anchor) {
  switch (anchor) {
    case geode::Anchor::TopLeft: return {0, 1};
    case geode::Anchor::Top: return {0.5f, 1};
    case geode::Anchor::TopRight: return {1, 1};
    case geode::Anchor::Right: return {1, 0.5f};
    case geode::Anchor::BottomRight: return {1, 0};
    case geode::Anchor::Bottom: return {0.5f, 0};
    case geode::Anchor::BottomLeft: return {0, 0};
    case geode::Anchor::Left: return {0, 0.5f};
    default: return {0, 0}; // Anchor::Center
  }
}
void FillFlowContainer::updateLayout() {
  float currentPos = 0;
  float maxCrossAxisSize = 0;
  auto children = geode::cocos::CCArrayExt<cocos2d::CCNode>(this->getChildren());

  auto anchorPoint = getAnchorPointFromAnchor(m_anchor);

  if (m_direction == FillDirection::Horizontal) {
    auto callback = [&](CCNode* child) {
      child->setAnchorPoint(anchorPoint);
      Container* e = geode::cast::typeinfo_cast<Container*>(child);
      if (!e) return;
      e->setPositionX(currentPos);
      e->setAnchor(m_anchor);
      e->updateContainerBox();
      // as for why this instead of just adding directly, negative size exists.
      currentPos = std::max(currentPos, currentPos + child->getContentSize().width + processUnit(getGap(), Unit::UIKit, true));
      maxCrossAxisSize = std::max(child->getContentSize().height, maxCrossAxisSize);
    };
    if (m_axisReverse) {
      auto iter = children.rbegin();
      auto end = children.rend();
      for (;iter != end; iter++) {
        auto child = *iter;
        callback(child);
      }
    } else {
      for (auto child : children) {
        callback(child);
      }
    }
    if (m_autoResize) this->setContentSize({currentPos, maxCrossAxisSize});
  } else {
    auto callback = [&](CCNode* child) {
      child->setAnchorPoint(anchorPoint);
      Container* e = geode::cast::typeinfo_cast<Container*>(child);
      if (!e) return;
      e->setPositionY(currentPos);
      e->setAnchor(m_anchor);
      e->updateContainerBox();
      currentPos = std::max(currentPos, currentPos + child->getContentSize().height + processUnit(getGap(), Unit::UIKit, false));
      maxCrossAxisSize = std::max(child->getContentSize().width, maxCrossAxisSize);
    };
    if (getAxisReverse()) {
      auto iter = children.rbegin();
      auto end = children.rend();
      for (;iter != end; iter++) {
        auto child = *iter;
        callback(child);
      }
    } else {
      for (auto child : children) {
        callback(child);
      }
    }
    if (m_autoResize) this->setContentSize({maxCrossAxisSize, currentPos});
  }
  this->updateContainerBox();
}
GDF_NS_END
