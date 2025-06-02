#include "FillFlowLayout.hpp"
#include "Container.hpp"

GDF_NS_START
FillFlowLayout* FillFlowLayout::create(FillDirection direction, geode::Anchor anchor) {
  auto ret = new FillFlowLayout(direction, anchor);
  ret->autorelease();
  return ret;
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

void FillFlowLayout::apply(cocos2d::CCNode* on) {
  auto container = geode::cast::typeinfo_cast<Container*>(on);
  if (!container) return;

  float currentPos = 0;
  float maxCrossAxisSize = 0;
  auto children = geode::cocos::CCArrayExt<cocos2d::CCNode>(container->getChildren());

  auto anchorPoint = getAnchorPointFromAnchor(m_anchor);

  if (m_direction == FillDirection::Horizontal) {
    auto callback = [&](CCNode* child) {
      child->setAnchorPoint(anchorPoint);
      Container* e = geode::cast::typeinfo_cast<Container*>(child);
      if (!e) return;
      e->setPositionX(currentPos);
      e->setAnchor(m_anchor);
      e->updateContainerBox();
      currentPos += child->getContentSize().width + getGap();
      maxCrossAxisSize = std::max(child->getContentSize().height, maxCrossAxisSize);
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
    if (getAutoGrowAxis().has_value()) on->cocos2d::CCNode::setContentSize({currentPos, maxCrossAxisSize});
  } else {
    auto callback = [&](CCNode* child) {
      child->setAnchorPoint(anchorPoint);
      Container* e = geode::cast::typeinfo_cast<Container*>(child);
      if (!e) return;
      e->setPositionY(currentPos);
      e->setAnchor(m_anchor);
      e->updateContainerBox();
      currentPos += child->getContentSize().height + getGap();
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
    if (getAutoGrowAxis().has_value()) on->cocos2d::CCNode::setContentSize({maxCrossAxisSize, currentPos});
  }
  container->updateContainerBox();
}
GDF_NS_END
