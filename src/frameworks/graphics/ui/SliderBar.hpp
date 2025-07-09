#pragma once

#include "../containers/Container.hpp"
#include "../../bindables/Bindable.hpp"

GDF_NS_START

class SliderBar : public Container {
protected:
  $kwargsStruct(
    Bindable<float>, value, 0.0f,
    float, minValue, 0.0f,
    float, maxValue, 1.0f
  );
public:
  static SliderBar* create(GDF_KWARGS) {
    $createClass(SliderBar, init, args);
  }

  bool init(GDF_KWARGS);

  void setValue(float value);
  float getValue() const;

  void setMinValue(float minValue);
  float getMinValue() const;

  void setMaxValue(float maxValue);
  float getMaxValue() const;
};

GDF_NS_END