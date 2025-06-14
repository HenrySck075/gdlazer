#pragma once

#include "../ui/OsuTabControl.hpp"
#include "../../../frameworks/graphics/color/Color4.hpp"

GDL_NS_START

class TabContainer : public frameworks::FillFlowContainer {
public:
  static TabContainer* create(std::vector<frameworks::Container*> items) {
    $createClass(TabContainer, init, items);
  }

  bool init(std::vector<frameworks::Container *> items);

private:
  OsuTabControl* m_control;
  std::map<frameworks::TabItem*, frameworks::Container*> m_items;
};

GDL_NS_END