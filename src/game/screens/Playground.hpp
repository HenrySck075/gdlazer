#pragma once
#include "../../framework/screens/Screen.hpp"
#include "../overlays/settings/SettingsButton.hpp"
#include "../graphics/OsuColor.hpp"
#include "../../utils.hpp"

class Playground : public Screen {
public:
    bool init() {
        if (!Screen::init()) return false;
        title("Widgets playground");
        addChild(SettingsButton::create("me", OsuColor::PinkDark));
        return true;
    }
    default_create(Playground);
};
