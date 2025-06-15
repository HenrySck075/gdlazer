#include "TabControl.hpp"

GDF_NS_START

bool TabFillFlowContainer::initWithItems(std::vector <TabItem *> items) {
  if (!FillFlowContainer::init(FillDirection::Horizontal))
    return false;
  for (auto i : items) {
    addChild(i);
  }
  return true;
}
bool TabControl::init(std::vector<TabItem *> items) {
  if (!Container::init())
    return false;
  addChild(m_container = TabFillFlowContainer::create(items));
  setContentSize({100, 100}, Unit::Percent);
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