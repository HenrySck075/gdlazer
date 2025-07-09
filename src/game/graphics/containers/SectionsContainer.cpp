#include "SectionsContainer.hpp"

GDF_NS_START
bool SectionsContainer::init() {
  m_contentsContainer = FillFlowContainer::create(FillDirection::Vertical);
  m_contentsContainer->setGap(4);
  if (!ScrollContainer::init(m_contentsContainer)) return false;
  m_currentSection.addCallback(std::bind(&SectionsContainer::onSectionSelect, this, std::placeholders::_1));

  return true;
}

void SectionsContainer::addSection(Container* section) {
  m_contentsContainer->addChild(section);
  section->addListener<MouseEvent>(std::bind(&SectionsContainer::sectionClickingShenanigans, this, std::placeholders::_1));
}

void SectionsContainer::onSectionSelect(Container* new_) {
  scrollTo(new_->getPosition());
}

void SectionsContainer::updateLayout() {
  m_contentsContainer->updateLayout();
}
bool SectionsContainer::sectionClickingShenanigans(MouseEvent* event) {
  m_currentSection = static_cast<Container*>(event->m_sender.operator->());
  return true;
};

GDF_NS_END
