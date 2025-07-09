#pragma once

#include "../../../../frameworks/graphics/ui/SliderBar.hpp"

GDL_NS_START
class SettingsSlider : public frameworks::SliderBar {
private:
  frameworks::Container* m_sliderBg;
public:
  static SettingsSlider* create(GDF_KWARGS) {
    $createClass(SettingsSlider, init, args);
  }

  bool init(GDF_KWARGS);
};

GDL_NS_END