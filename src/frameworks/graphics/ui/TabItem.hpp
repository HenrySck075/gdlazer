#pragma once

#include "../containers/ClickableContainer.hpp"

GDF_NS_START

class TabItem : public ClickableContainer {
public:
  static TabItem* create() {
    $createClass(TabItem, init);
  };
  bool init() override {
    if (!ClickableContainer::init("", true)) return false;
    setContentHeight(100, Unit::Percent);
    return true;
  }
private:
};

GDF_NS_END