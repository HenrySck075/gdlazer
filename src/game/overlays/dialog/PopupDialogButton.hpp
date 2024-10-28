#pragma once

#include <Geode/Geode.hpp>
#include "../../../framework/graphics/containers/ClickableContainer.hpp"
#include "../../../helpers/CustomActions.hpp"

using namespace geode::prelude;

class PopupDialogButton : public ClickableContainer {
private:

  const float idle_width = 0.8f;
  const float hover_width = 0.9f;

  const float hover_duration = 0.3f;
  const float click_duration = 0.2f;

  float height = 50;
  ccColor3B m_color;
  const char* m_sfx;

  void onMouseEnter() override;
  void onMouseExit() override;
  bool init(const char* label, ccColor3B color, const char* clickSfx, ButtonCallback clickCb);
  void onMouseDown(MouseEvent* event) override;
  void onMouseUp(MouseEvent* event) override;
  //void onClick(MouseEvent* e) override {};
public:
  void setOpacity(GLubyte opacity) override;
  static PopupDialogButton* create(const char* label, ccColor3B color, const char* clickSfx, ButtonCallback clickCb);
  void setContentHeight(float height);
};

