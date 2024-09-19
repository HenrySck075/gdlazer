#include "SectionsContainer.hpp"

bool SectionsContainer::init() {
    contentsContainer = FillFlowContainer::create(FillDirection::Vertical);
    if (!ScrollableContainer::init(contentsContainer)) return false;
    // can you be normal
    //currentSection.addCallback(&SectionsContainer::onSectionSelect);
    currentSection.addCallback(std::bind(&SectionsContainer::onSectionSelect, this, std::placeholders::_1, std::placeholders::_2));
    return true;
}

void SectionsContainer::addChild(CCNode* node) {
    contentsContainer->addChild(node);
}

void SectionsContainer::onSectionSelect(Container* old, Container* new_) {
    moveToPos(new_->getPosition());
}
