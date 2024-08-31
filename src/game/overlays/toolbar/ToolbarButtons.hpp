#pragma once

#include "ToolbarButton.hpp"


class ToolbarSettingsButton : public ToolbarButton {
public:
    static ToolbarSettingsButton* create() {
        create_class(ToolbarSettingsButton, init, OsuIcon::Settings, "Settings", "the");
    }
};
