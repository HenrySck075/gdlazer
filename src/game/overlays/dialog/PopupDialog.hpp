#ifndef __osu_ui_dialog__
#define __osu_ui_dialog__

#include <Geode/Geode.hpp>
#include "PopupDialogButton.hpp"
#include "../../../helpers/colors.hpp"

using namespace geode::prelude;

class PopupDialog : public geode::Popup<std::string const&, std::string const&, std::initializer_list<PopupDialogButton*>> {
private:
    CCClippingNode* m_bgSpriteClip;
    CCLayerRGBA* m_bodyLayout;
protected:
    bool setup(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons) override;


    void onClose(cocos2d::CCObject*) override;

public:
    void show() override;
    void hide();
    static PopupDialog* create(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons) {
        auto ret = new PopupDialog();
        if (ret->initAnchored(330.f, 300.f, title, content, buttons, "dialog.png"_spr)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
    static PopupDialog* createSimpleDialog(std::string const& title, std::string const& content, std::string const& confirmButtonText, std::string const& cancelButtonText, ButtonCallback confirmCallback) {
      auto b = PopupDialog::create(
          title, 
          content, 
          {
            PopupDialogButton::create(confirmButtonText.c_str(), dialog_button_primary, "idk.wav", confirmCallback),
            PopupDialogButton::create(cancelButtonText.c_str(), dialog_button_secondary, "h", &PopupDialog::hide)
          });
      return b;
    }
    void keyBackClicked() override;
};
#endif
