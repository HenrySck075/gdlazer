#include "TabControl.hpp"

GDF_NS_START

bool TabFillFlowContainer::initWithItems(std::vector <TabItem *> items) {
  if (!FillFlowContainer::init({}))
    return false;
  for (auto i : items) {
    this->addChild(i);
  }
  for (auto i : items) {
    m_items.push_back(i);
  }
  return true;
}
bool TabControl::init(std::vector<TabItem *> items) {
  if (!Container::init())
    return false;
  this->addChild(m_container = TabFillFlowContainer::create(items));
  this->setContentSize({100, 100}, Unit::Percent);
  for (auto tab : items) {
    tab->addListener<MouseEvent>([this, tab](MouseEvent *e) {
      if (e->m_eventType == MouseEventType::Click) {
        m_selectedTab = tab;
      }
      return true;
    });
  }
  m_selectedTab = items[0];
  return true;
}

GDF_NS_END