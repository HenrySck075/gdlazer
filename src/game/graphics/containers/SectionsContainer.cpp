#include "SectionsContainer.hpp"

GDF_NS_START
bool SectionsContainer::init() {
  m_contentsContainer = FillFlowContainer::create(FillDirection::Vertical);
  m_contentsContainer->setGap(4);
  if (!ScrollContainer::init(m_contentsContainer)) return false;
  m_currentSection.addCallback(std::bind(&SectionsContainer::onSectionSelect, this, std::placeholders::_1, std::placeholders::_2));

  m_questionableCode = std::bind(&SectionsContainer::sectionClickingShenanigans, this, std::placeholders::_1);

  m_contentsContainer->addListener<NodeSizeUpdated>([this](NodeSizeUpdated*) {
    setContentSize(m_contentsContainer->getContentSize());
    return true;
  });

  m_contentsContainer->setAnchorPoint({0,1});

  setContentSize({100,100}, Unit::Percent);

  return true;
}

void SectionsContainer::addSection(Container* section) {
  m_contentsContainer->addChild(section);
  section->addListener(m_questionableCode);
  m_contentsContainer->updateLayout();
}

void SectionsContainer::onSectionSelect(Container*, Container* new_) {
  scrollTo(new_->getPosition());
}

void SectionsContainer::updateLayout() {
}
bool SectionsContainer::sectionClickingShenanigans(MouseEvent* event) {
  m_currentSection = static_cast<Container*>(event->m_sender.operator->());
  return true;
};

GDF_NS_END
