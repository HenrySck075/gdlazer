#include "SettingsPanel.hpp"

const float SettingsPanel::sidebar_width = SettingsSidebar::EXPANDED_WIDTH;
const float SettingsPanel::TRANSITION_LENGTH = 0.6;
const float SettingsPanel::PANEL_WIDTH = 400;
const float SettingsPanel::WIDTH = SettingsPanel::sidebar_width + SettingsPanel::PANEL_WIDTH;

bool SettingsPanel::init() {
    if (!Container::init()) return false;

    return true;
}

void SettingsPanel::onOpen() {}
void SettingsPanel::onClose() {}
