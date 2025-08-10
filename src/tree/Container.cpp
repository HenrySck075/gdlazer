#include "Container.hpp"
#include "Element.hpp"

cocos2d::CCNode* Element<Container>::build() {
  auto node = CCNode::create();
  node->setContentSize({widget.size.x});
};
