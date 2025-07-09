#include "SettingsSlider.hpp"
#include "../../../graphics/OsuColor.hpp"

GDL_NS_START
using namespace frameworks;
bool SettingsSlider::init(GDF_KWARGS) {
  if (!SliderBar::init(args))
    return false;

  addListener<NodeSizeUpdated>([this](NodeSizeUpdated *) {
    setBorderRadius(getContentHeight() / 2);
    return true;
  });

  setContentHeight(12, Unit::UIKit);

  addChild(m_sliderBg = frameworks::Container::create());
  m_sliderBg->setBackgroundColor(OsuColor::Pink);
  m_sliderBg->setContentHeight(100, Unit::Percent);
  m_value.addCallback([this](float oldV, float newV) {
    // Update slider position based on value
    float percentage = (newV - m_minValue) / (m_maxValue - m_minValue);
    m_sliderBg->setContentWidth(percentage, Unit::Percent);
  });

  return true;
}

GDL_NS_END