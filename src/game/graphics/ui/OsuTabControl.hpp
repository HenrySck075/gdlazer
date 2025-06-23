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

  /// TODO: move this to frameworks
  void focus();
  void unfocus();

  constexpr static const float MIN_WIDTH = 150;
  constexpr static const float HEIGHT = 50;
private:
  std::string m_text;
  Container* m_bar;
  OsuText* m_textNode;
  ccColor4B m_accent;
  bool m_focused = false;
};


class OsuTabControl : public frameworks::TabControl {
public: friend class TabContainer;
  static OsuTabControl* create(std::vector<OsuTabItem*> items) {
    $createClass(OsuTabControl, init, items);
  }

  bool init(std::vector<OsuTabItem *> items);
};

GDL_NS_END