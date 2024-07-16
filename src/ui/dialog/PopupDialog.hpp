#ifndef __osu_ui_dialog__
#define __osu_ui_dialog__

#include "Geode/Geode.hpp"
#include "../../utils.hpp"
#include "../../helpers/colors.hpp"
#include "../deco/Triangles.hpp"
#include "DialogButton.hpp"

using namespace geode::prelude;

class PopupDialog : public geode::Popup<std::string const&, std::string const&, std::initializer_list<DialogButton*>> {
private:
    CCClippingNode* m_bgSpriteClip;
    CCLayerRGBA* m_bodyLayout;
protected:
    bool setup(std::string const& title, std::string const& content, std::initializer_list<DialogButton*> buttons) override;


    void onClose(cocos2d::CCObject*) override;

public:
    void show() override;
    static PopupDialog* create(std::string const& title, std::string const& content, std::initializer_list<DialogButton*> buttons) {
        auto ret = new PopupDialog();
        if (ret->initAnchored(220.f, 200.f, title, content, buttons, "dialog.png"_spr)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
    void keyBackClicked() override;
};
#endif
