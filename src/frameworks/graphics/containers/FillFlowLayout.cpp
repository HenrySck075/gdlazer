#include "FillFlowLayout.hpp"
#include "Container.hpp"

FillFlowLayout* FillFlowLayout::create(FillDirection direction) {
    auto ret = new FillFlowLayout(direction);
    ret->autorelease();
    return ret;
}

void FillFlowLayout::apply(cocos2d::CCNode* on) {
    auto container = dynamic_cast<Container*>(on);
    if (!container) return;

    float currentPos = 0;
    auto children = geode::cocos::CCArrayExt<cocos2d::CCNode>(container->getChildren());
    
    if (m_direction == FillDirection::Horizontal) {
        for (auto child : children) {
            if (!child) continue;
            child->setPositionX(currentPos);
            currentPos += child->getContentSize().width;
        }
    } else {
        for (auto child : children) {
            if (!child) continue;
            child->setPositionY(currentPos);
            currentPos += child->getContentSize().height;
        }
    }
}