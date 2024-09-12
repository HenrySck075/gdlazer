#include "ToolbarButtons.hpp"
#include "../../OsuGame.hpp"

/**
 * ToolbarSettingsButton
 */

void ToolbarSettingsButton::select() {
    OsuGame::get()->showSettings();
}

void ToolbarSettingsButton::deselect() {
    OsuGame::get()->hideSettings();
}