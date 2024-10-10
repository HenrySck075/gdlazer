#include "ToolbarButtons.hpp"
#include "../../OsuGame.hpp"

/**
 * ToolbarSettingsButton
 */

void ToolbarSettingsButton::select() {
    ToolbarToggleButton::select();
    OsuGame::get()->showSettings();
}

void ToolbarSettingsButton::deselect() {
    ToolbarToggleButton::deselect();
    OsuGame::get()->hideSettings();
}


/**
 * ToolbarMusicButton
 */

void ToolbarMusicButton::select() {
    ToolbarToggleButton::select();
    OsuGame::get()->pushOverlay(o);
}

void ToolbarMusicButton::deselect() {
    ToolbarToggleButton::deselect();
    OsuGame::get()->popOverlay(o);
}
