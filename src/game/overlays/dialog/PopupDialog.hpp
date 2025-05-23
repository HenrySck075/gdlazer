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
  geode::Ref<CCClippingNode> m_bgSpriteClip;
  geode::Ref<CCNodeRGBA> m_bodyLayout;
  geode::Ref<frameworks::FillFlowContainer> m_btnLayer;
  geode::Ref<CCScale9Sprite> m_bgSprite;
  geode::Ref<OsuText> m_title;
  geode::cocos::CCArrayExt<PopupDialogButton> m_buttons;

protected:
  //void onClose(cocos2d::CCObject*) override;

public:
  static float width;
  static float height;

  void onOpen() override;
  void onClose() override;
  bool init(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons);
  bool init2(std::string const &title, std::string const &content,
             std::string const &confirmButtonText,
             std::string const &cancelButtonText,
             frameworks::ButtonCallback confirmCallback);
  static PopupDialog* create(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons) {
    $create_class(PopupDialog, init, title, content, buttons);
  }
  static PopupDialog* createSimpleDialog(std::string const& title, std::string const& content, std::string const& confirmButtonText, std::string const& cancelButtonText, frameworks::ButtonCallback confirmCallback) {
    $create_class(PopupDialog, init2, title, content, confirmButtonText, cancelButtonText, confirmCallback);
  }
  void onDismiss() override {}
};
GDL_NS_END
