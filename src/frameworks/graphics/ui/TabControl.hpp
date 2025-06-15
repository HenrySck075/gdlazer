#pragma once

#include "../containers/Container.hpp"
#include "../containers/FillFlowContainer.hpp"
#include "TabItem.hpp"

GDF_NS_START


class TabFillFlowContainer : public FillFlowContainer {
public:
  static TabFillFlowContainer* create(std::vector<TabItem*> items) {
    $createClass(TabFillFlowContainer, initWithItems, items);
  };
  bool initWithItems(std::vector<TabItem *> items);

private:
  friend class TabControl;
  std::vector<TabItem*> m_items;
};

class TabControl : public Container {
public:
  static TabControl* create(std::vector<TabItem*> items) {
    $createClass(TabControl, init, items);
  }

  bool init(std::vector<TabItem *> items);

  void setSpacing(float spacing) {m_container->setGap(spacing);}

  std::vector<TabItem*> getAllTabs() {return m_container->m_items;};
  Bindable<TabItem*> m_selectedTab;
protected:
  TabFillFlowContainer* m_container;
private:
  bool m_manual = false;
};

GDF_NS_END