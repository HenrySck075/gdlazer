#pragma once

#include "../../../frameworks/graphics/containers/ClickableContainer.hpp"
#include "../../../frameworks/graphics/color/Color4.hpp"

GDL_NS_START

class OsuButton : public frameworks::ClickableContainer {
  std::string m_text;
  Color4 m_backgroundColor;

  GDF_KWARGS_STRUCT(
    text,
    backgroundColor
  );
public:
  static OsuButton* create(GDF_KWARGS) {
    $createClass2(OsuButton);
  }

  bool init(GDF_KWARGS);
};

GDL_NS_END