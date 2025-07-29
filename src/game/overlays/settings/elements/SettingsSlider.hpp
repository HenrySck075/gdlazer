#pragma once

#include "../../../../frameworks/graphics/ui/SliderBar.hpp"

GDL_NS_START
class SettingsSlider : public frameworks::SliderBar {
private:
  geode::Ref<frameworks::Container> m_sliderBg;
  geode::Ref<frameworks::Container> m_bg;
public:
  static SettingsSlider* create(GDF_KWARGS) {
    $createClass(SettingsSlider, init, args);
  }

  bool init(GDF_KWARGS);
  void convoluted(float);
};

GDL_NS_END