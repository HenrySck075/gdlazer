#include "SectionsContainer.hpp"

GDF_NS_START
bool SectionsContainer::init() {
  contentsContainer = FillFlowContainer::create(FillDirection::Vertical);
  if (!ScrollContainer::init(contentsContainer)) return false;
  // can you be normal
  //currentSection.addCallback(&SectionsContainer::onSectionSelect);
  currentSection.addCallback(std::bind(&SectionsContainer::onSectionSelect, this, std::placeholders::_1));
  return true;
}

void SectionsContainer::addChild(CCNode* node) {
  contentsContainer->addChild(node);
}

void SectionsContainer::onSectionSelect(Container* new_) {
  scrollTo(new_->getPosition());
}
GDF_NS_END
