#pragma once

#include <Geode/Geode.hpp>
#include "PopupDialogButton.hpp"
#include "../../graphics/containers/OsuOverlayContainer.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../graphics/ui/OsuText.hpp"

using namespace geode::prelude;

GDL_NS_START
class PopupDialog : public OsuOverlayContainer {
private:
  float volume;   
  bool hiding = false;
  geode::Ref<CCClippingNodeRGBA> m_bgSpriteClip;
  geode::Ref<CCNodeRGBA> m_bodyLayout;
  geode::Ref<frameworks::FillFlowContainer> m_btnLayer;
  geode::Ref<CCScale9Sprite> m_bgSprite;
  geode::Ref<OsuText> m_title;
  geode::cocos::CCArrayExt<PopupDialogButton> m_buttons;

  float m_lowPassStrength = 0;

protected:
  //void onClose(cocos2d::CCObject*) override;

public:
  static float width;
  static float height;

  void onOpen() override;
  void onClose() override;
  bool init(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons, float strength);
  bool init2(std::string const &title, std::string const &content, float strength,
             std::string const &confirmButtonText,
             std::string const &cancelButtonText,
             frameworks::ButtonCallback confirmCallback);
  void setLowPassStrength(float strength) {
    m_lowPassStrength = strength;
  }
  static PopupDialog* create(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons, float strength = 0) {
    $createClass(PopupDialog, init, title, content, buttons, strength);
  }
  static PopupDialog* createSimpleDialog(std::string const& title, std::string const& content, float strength = 0, std::string const& confirmButtonText = "Yes, go ahead", std::string const& cancelButtonText = "sybau", frameworks::ButtonCallback confirmCallback = [](CCNode*){}) {
    $createClass(PopupDialog, init2, title, content, strength, confirmButtonText, cancelButtonText, confirmCallback);
  }
  void onDismiss() override {}
};
GDL_NS_END
