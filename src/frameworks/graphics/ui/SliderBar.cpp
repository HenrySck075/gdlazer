#include "SliderBar.hpp"

GDF_NS_START

bool SliderBar::init(GDF_KWARGS) {
  if (!Container::init()) return false;

  ctorInitArgs(args);

  /// Use a mouse listener to set the value based on where the mouse clicks in the x axis
  this->addListener<MouseEvent>([this](MouseEvent* event) {
    if (event->m_eventType == MouseEventType::MouseDown) {
      auto pos = event->m_position;
      float newValue = (pos.x / this->getContentWidth()) * (m_maxValue - m_minValue) + m_minValue;
      setValue(newValue);
      return true;
    }
    return false;
  });
  
  return true;
}

void SliderBar::setValue(float value) {
  m_value = std::clamp(value, m_minValue, m_maxValue);
}
float SliderBar::getValue() const {
  return m_value.operator->();
}
void SliderBar::setMinValue(float minValue) {
  m_minValue = minValue;
  setValue(m_value); // Ensure current value is within new bounds
}
float SliderBar::getMinValue() const {
  return m_minValue;
}
void SliderBar::setMaxValue(float maxValue) {
  m_maxValue = maxValue;
  setValue(m_value); // Ensure current value is within new bounds
}
float SliderBar::getMaxValue() const {
  return m_maxValue;
}

GDF_NS_END