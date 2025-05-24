#pragma once

#include <Geode/Geode.hpp>
#include "../../../frameworks/graphics/containers/ClickableContainer.hpp"

using namespace geode::prelude;

GDL_NS_START
class PopupDialogButton : public frameworks::ClickableContainer {
private:
  geode::Ref<CCLayerGradient> m_gradLeft;
  geode::Ref<CCLayerGradient> m_gradRight;
  geode::Ref<CCNodeRGBA> m_grimace;

  const float c_idleWidth = 0.8f;
  const float c_hoverWidth = 0.9f;

  const float c_hoverDuration = 0.3f;
  const float c_clickDuration = 0.2f;

  float m_height = 50;
  ccColor3B m_color;
  std::string m_sfx;

  bool init(std::string label, ccColor3B color, std::string clickSfx, frameworks::ButtonCallback clickCb);
public:
  static PopupDialogButton* create(std::string label, ccColor3B color, std::string clickSfx, frameworks::ButtonCallback clickCb);
  void setContentHeight(float height);
  // i love when opacity cascading DOESNT FUCKING WORK!!!!!!!!!!!
  void updateDisplayedOpacity(GLubyte parentOpacity) override;
  void onEnter() override;
  void onExit() override;
};
GDL_NS_END
