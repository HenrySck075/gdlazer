#pragma once

#include "ToolbarButton.hpp"
#include "ToolbarToggleButton.hpp"
#include "../../../framework/input/events/KeyEvent.hpp"
#include "../NowPlayingOverlay.hpp"

class ToolbarSettingsButton : public ToolbarToggleButton {
public:
    bool init() {
        setID("settings");
        addListener("keyboardEvent",[this](NodeEvent* ev){
            auto e = static_cast<KeyboardEvent*>(ev);
            if (e->key.ctrl && e->key.key == enumKeyCodes::KEY_O) select();
        });
        return ToolbarToggleButton::init(OsuIcon::Settings, "settings", "the");
    }
    default_create(ToolbarSettingsButton);
    void select() override;
    void deselect() override;
};

class ToolbarHomeButton : public ToolbarButton {
public:
    static ToolbarHomeButton* create() {
        create_class(ToolbarHomeButton, init);
    }
    bool init() {
        setID("home");
        return ToolbarButton::init(OsuIcon::Home, "home", "return to the main menu");
    }
};


//

class ToolbarGeodeButton : public ToolbarButton {
public:
    default_create(ToolbarGeodeButton);
    bool init() { 
        return ToolbarButton::init(
            OsuIcon::Gear,
            "geode mod loader",
            "manages the mods (F12)",
            AxisAlignment::End
        );
    }
    void onClick(MouseEvent* e) override {
        ToolbarButton::onClick(e);
        static_cast<CCMenuItemSpriteExtra*>(
            GameManager::sharedState()->m_menuLayer->getChildByIDRecursive("geode.loader/geode-button")
        )->activate();
    }
};

class ToolbarMusicButton : public ToolbarToggleButton {
    NowPlayingOverlay* o;
public:
    default_create(ToolbarMusicButton);
    bool init() {
        setID("music");
        o = NowPlayingOverlay::create();
        o->retain();
        return ToolbarToggleButton::init(
            OsuIcon::Music, 
            "now playing", 
            "manage the currently playing track (F6)", 
            AxisAlignment::End
        );
    }
    void select() override;
    void deselect() override;

    ~ToolbarMusicButton() {
        o->release();
    }
};
