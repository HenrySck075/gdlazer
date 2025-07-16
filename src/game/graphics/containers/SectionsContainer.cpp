#include "SectionsContainer.hpp"

GDF_NS_START
bool SectionsContainer::init() {
  m_contentsContainer = FillFlowContainer::create({.direction=FillDirection::Vertical, .gap=4.f});
  if (!ScrollContainer::init(m_contentsContainer)) return false;
  m_currentSection.addCallback(std::bind(&SectionsContainer::onSectionSelect, this, std::placeholders::_1, std::placeholders::_2));

  m_questionableCode = std::bind(&SectionsContainer::sectionClickingShenanigans, this, std::placeholders::_1);

  m_contentsContainer->addListener<NodeSizeUpdated>([this](NodeSizeUpdated*) {
    this->setContentSize(m_contentsContainer->getContentSize());
    return true;
  });

  m_contentsContainer->setAnchorPoint({0,1});

  this->setContentSize({100,100}, Unit::Percent);

  return true;
}

void SectionsContainer::addSection(Container* section) {
  m_contentsContainer->addChild(section);
  section->addListener(m_questionableCode);
}

void SectionsContainer::onSectionSelect(Container*, Container* new_) {
  this->scrollTo(new_->getPosition());
}

void SectionsContainer::updateLayout() {
}
bool SectionsContainer::sectionClickingShenanigans(MouseEvent* event) {
  m_currentSection = static_cast<Container*>(event->m_sender.operator->());
  return true;
};
void SectionsContainer::onEnter() {
  m_contentsContainer->updateLayout();
}

GDF_NS_END
