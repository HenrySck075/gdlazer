#pragma once
#include "../../framework/screens/Screen.hpp"
#include "../overlays/settings/SettingsButton.hpp"
#include "../graphics/ui/LoadingSpinner.hpp"
#include "../graphics/OsuColor.hpp"
#include "../../utils.hpp"

class Playground final : public Screen {
public:
    bool init() {
        if (!Screen::init()) return false;
        title("Widgets playground");
        addChild(SettingsButton::create("me", OsuColor::PinkDark));

        auto spinner = LoadingSpinner::create(false, true);
        spinner->setPosition({50,150});
        spinner->setContentSize({50,50});
        addChild(spinner);
        spinner->show();

        return true;
    }
    default_create(Playground);
};
