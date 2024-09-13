#pragma once

#include "ToolbarButton.hpp"
#include "ToolbarToggleButton.hpp"

class ToolbarSettingsButton : public ToolbarToggleButton {
public:
    bool init() {
        setID("settings");
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
