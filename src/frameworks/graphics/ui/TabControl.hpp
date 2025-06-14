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
  bool initWithItems(std::vector<TabItem*> items) {
    if (!FillFlowContainer::init(FillDirection::Horizontal)) return false;
    for (auto i : items) {
      addChild(i);
    }

    return true;
  };
private:
  friend class TabControl;
  std::vector<TabItem*> m_items;
};

class TabControl : public Container {
public:
  static TabControl* create(std::vector<TabItem*> items) {
    $createClass(TabControl, init, items);
  }

  bool init(std::vector<TabItem*> items) {
    if (!Container::init()) return false;
    addChild(m_container = TabFillFlowContainer::create(items));
    setContentSize({100,100}, Unit::Percent);
    for (auto tab : items) {
      tab->addListener<MouseEvent>([this, tab](MouseEvent* e) {
        if (e->m_eventType == MouseEventType::Click) {
          m_selectedTab = tab;
        }
        return true;
      });
    }
    m_selectedTab = items[0];
    return true;
  }

  void setSpacing(float spacing) {m_container->setGap(spacing);}

  std::vector<TabItem*> getAllTabs() {return m_container->m_items;};
  Bindable<TabItem*> m_selectedTab;
protected:
private:
  bool m_manual = false;
  TabFillFlowContainer* m_container;
};

GDF_NS_END