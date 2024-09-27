#pragma once

#include <Geode/Geode.hpp>
#include "PopupDialogButton.hpp"
#include "../../../helpers/colors.hpp"
#include "../../../utils.hpp"
#include "../../../framework/graphics/containers/EventTarget.hpp"
#include "../../graphics/containers/OsuOverlayContainer.hpp"

using namespace geode::prelude;

/// TODO: Use OverlaysContainer
class PopupDialog : public OsuOverlayContainer, public EventTarget {
private:
    float volume;   
    bool hiding = false;
    CCClippingNode* m_bgSpriteClip;
    CCLayerRGBA* m_bodyLayout;
    std::vector<PopupDialogButton*> m_buttons;


protected:
    bool init(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons);
    //void onClose(cocos2d::CCObject*) override;

public:
    static float width;
    static float height;

    void onOpen() override;
    void onClose() override;
    bool init2(std::string const& title, std::string const& content, std::string const& confirmButtonText, std::string const& cancelButtonText, ButtonCallback confirmCallback) {
        return init(title, content, {
            PopupDialogButton::create(confirmButtonText.c_str(), dialog_button_primary, "dialog-ok-select.wav"_spr,[this,confirmCallback](CCNode* the) {confirmCallback(the); hide(); }),
            PopupDialogButton::create(cancelButtonText.c_str(), dialog_button_secondary, "dialog-cancel-select.wav"_spr, [this](CCNode* self) {hide(); })
        });
    }
    static PopupDialog* create(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons) {
        create_class(PopupDialog, init, title, content, buttons);
    }
    static PopupDialog* createSimpleDialog(std::string const& title, std::string const& content, std::string const& confirmButtonText, std::string const& cancelButtonText, ButtonCallback confirmCallback) {
        create_class(PopupDialog, init2, title, content, confirmButtonText, cancelButtonText, confirmCallback);
    }
    void keyBackClicked() override;
};
