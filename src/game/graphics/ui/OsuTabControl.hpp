#pragma once

#include "../../../frameworks/graphics/ui/TabControl.hpp"
#include "../../../frameworks/graphics/ui/TabItem.hpp"
#include "OsuText.hpp"

GDL_NS_START
class OsuTabItem : public frameworks::TabItem {
public:
  static OsuTabItem* create(std::string text, ccColor4B accent) {
    $createClass(OsuTabItem, init, text, accent);
  }

  bool init(std::string text, ccColor4B accent);

  void fadeIn();

  void fadeOut();

  constexpr static const float MIN_WIDTH = 150;
private:
  std::string m_text;
  Container* m_bar;
  OsuText* m_textNode;
  ccColor4B m_accent;
};


class OsuTabControl : public frameworks::TabControl {
public: friend class TabContainer;
  static OsuTabControl* create(std::vector<OsuTabItem*> items) {
    $createClass(OsuTabControl, init, items);
  }

  bool init(std::vector<OsuTabItem*> items) {
    if (!frameworks::TabControl::init({items.begin(), items.end()})) return false;
    setSpacing(5);
    m_selectedTab.addCallback([](frameworks::TabItem* old, frameworks::TabItem* new_){
      static_cast<OsuTabItem*>(old)->fadeOut();
      static_cast<OsuTabItem*>(new_)->fadeIn();
    });
    return false;
  }
};

GDL_NS_END