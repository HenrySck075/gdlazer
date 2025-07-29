#include "SettingsSlider.hpp"
#include "../../OverlayColorProvider.hpp"

GDL_NS_START
using namespace frameworks;
bool SettingsSlider::init(GDF_KWARGS) {
  if (!SliderBar::init(args))
    return false;

  this->addListener<NodeSizeUpdated>([this](NodeSizeUpdated *) {
    setBorderRadius(getContentHeight() / 2);
    return true;
  });

  this->setContentHeight(40, Unit::UIKit);


  this->addChild(m_sliderBg = frameworks::Container::create());
  m_sliderBg->setContentHeight(10, Unit::UIKit);
  m_sliderBg->setAnchor(Anchor::Left);
  m_sliderBg->setLogName("slider bar");

  this->addChild(m_bg = frameworks::Container::create(), -2);
  m_bg->setContentSize({100,10}, Unit::Percent, Unit::UIKit);
  m_bg->setAnchor(Anchor::Left);
  m_bg->setLogName("background");
  m_value.addCallback([this](float oldV, float newV) {
    // Update slider position based on value
    float percentage = (newV - m_minValue) / (m_maxValue - m_minValue);
    m_sliderBg->setContentWidth(percentage, Unit::Percent);
  });

  this->scheduleOnce(schedule_selector(SettingsSlider::convoluted),0);

  return true;
}
void SettingsSlider::convoluted(float) {
  auto provider = OverlayColorProvider::of(this);
  m_sliderBg->setBackgroundColor(provider->Foreground1());
  m_bg->setBackgroundColor(provider->Background5());
};

GDL_NS_END