#pragma once

#include "ToolbarButton.hpp"
#include "ToolbarToggleButton.hpp"

class ToolbarSettingsButton : public ToolbarToggleButton {
public:
    static ToolbarSettingsButton* create() {
        create_class(ToolbarSettingsButton, init, OsuIcon::Settings, "Settings", "the", ccColor3B(205,29,72));
    }
    void select() override {};
};

