#ifndef __osu_ui_dialog__
#define __osu_ui_dialog__

#include "Geode/Geode.hpp"
#include "../utils.hpp"
#include "../helpers/colors.hpp"
#include "deco/Triangles.hpp"
#include "DialogButton.hpp"

using namespace geode::prelude;

class osuDialog : public geode::Popup<std::string const&, std::string const&> {
private:
    CCClippingNode* m_bgSpriteClip;
    CCLayer* m_bodyLayout;
protected:
    bool setup(std::string const& title, std::string const& content) override;


    void onClose(cocos2d::CCObject*) override;

public:
    void show() override;
    static osuDialog* create(std::string const& title, std::string const& content) {
        auto ret = new osuDialog();
        if (ret->initAnchored(220.f, 200.f, title, content, "dialog.png"_spr)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};
#endif