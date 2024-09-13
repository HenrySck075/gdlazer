#pragma once

#include "ToolbarButton.hpp"
#include "ToolbarToggleButton.hpp"
#include "../../../framework/input/events/KeyEvent.hpp"

class ToolbarSettingsButton : public ToolbarToggleButton {
public:
    bool init() {
        setID("settings");
        addListener("keyboardEvent",[this](NodeEvent* ev){
            auto e = static_cast<KeyboardEvent*>(ev);
            if (e->key.ctrl && e->key.key == enumKeyCodes::KEY_O) select();
        });
        return ToolbarToggleButton::init(OsuIcon::Settings, "Settings", "the", {205,29,72});
    }
    static ToolbarSettingsButton* create() {
        create_class(ToolbarSettingsButton, init);
    }
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
        return ToolbarButton::init(OsuIcon::Home, "Home", "Return to the main menu");
    }
};
