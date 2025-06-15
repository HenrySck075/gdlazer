#include "TabContainer.hpp"

GDL_NS_START
bool TabContainer::init(std::vector<frameworks::Container *> items, Color4 accent) {
  if (!frameworks::FillFlowContainer::init(frameworks::FillDirection::Vertical,
                                           Anchor::TopLeft))
    return false;
  std::vector<OsuTabItem *> tabs;
  for (auto container : items) {
    auto tab = OsuTabItem::create(container->getName(), accent);
    tabs.push_back(tab);
    m_items[tab] = container;
  }
  addChild(m_control = $verifyPtr(OsuTabControl::create(tabs)));
  m_control->setContentHeight(OsuTabItem::HEIGHT, frameworks::Unit::UIKit);
  m_control->m_selectedTab.addCallback(
    [this](frameworks::TabItem *old, frameworks::TabItem *new_) {
      m_items[old]->removeFromParent();
      addChild(m_items[new_]);
      updateLayout();
    }
  );
  setAutoResize(false);
  addChild(m_items[m_control->m_selectedTab]);
  setContentSize({100, 100}, frameworks::Unit::Percent);
  setMinSize({OsuTabItem::MIN_WIDTH, OsuTabItem::HEIGHT + 20});
  return true;
}

GDL_NS_END