#include "FillFlowLayout.hpp"
#include "Container.hpp"

GDF_NS_START
FillFlowLayout* FillFlowLayout::create(FillDirection direction) {
  auto ret = new FillFlowLayout(direction);
  ret->autorelease();
  return ret;
}

void FillFlowLayout::apply(cocos2d::CCNode* on) {
  auto container = geode::cast::typeinfo_cast<Container*>(on);
  if (!container) return;

  float currentPos = 0;
  float maxCrossAxisSize = 0;
  auto children = geode::cocos::CCArrayExt<cocos2d::CCNode>(container->getChildren());
  
  if (m_direction == FillDirection::Horizontal) {
    for (auto child : children) {
      Container* e = geode::cast::typeinfo_cast<Container*>(child);
      if (!e) continue;
      e->setPositionX(currentPos);
      e->updateContainerBox();
      currentPos += child->getContentSize().width;
      maxCrossAxisSize = std::max(child->getContentSize().height, maxCrossAxisSize);
    }
    on->cocos2d::CCNode::setContentSize({currentPos, maxCrossAxisSize});
  } else {
    for (auto child : children) {
      Container* e = geode::cast::typeinfo_cast<Container*>(child);
      if (!e) continue;
      e->setPositionY(currentPos);
      e->updateContainerBox();
      currentPos += child->getContentSize().height;
      maxCrossAxisSize = std::max(child->getContentSize().width, maxCrossAxisSize);
    }
    on->cocos2d::CCNode::setContentSize({maxCrossAxisSize, currentPos});
  }
  // to call updateContainerBox
  container->setBorderRadius(container->getBorderRadius());
}
GDF_NS_END
