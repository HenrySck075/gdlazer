#pragma once

#include <Geode/Geode.hpp>
#include "../../../frameworks/graphics/containers/ClickableContainer.hpp"

using namespace geode::prelude;

GDL_NS_START
class PopupDialogButton : public frameworks::ClickableContainer {
private:
  CCLayerGradient* m_gradLeft = nullptr;
  CCLayerGradient* m_gradRight = nullptr;

  const float c_idleWidth = 0.8f;
  const float c_hoverWidth = 0.9f;

  const float c_hoverDuration = 0.3f;
  const float c_clickDuration = 0.2f;

  float m_height = 50;
  ccColor3B m_color;
  const char* m_sfx;

  bool init(const char* label, ccColor3B color, const char* clickSfx, ButtonCallback clickCb);
public:
  void setOpacity(GLubyte opacity) override;
  static PopupDialogButton* create(const char* label, ccColor3B color, const char* clickSfx, ButtonCallback clickCb);
  void setContentHeight(float height);
};
GDL_NS_END
