#pragma once
#include "../../framework/screens/Screen.hpp"
#include "../overlays/settings/SettingsButton.hpp"
#include "../graphics/ui/LoadingSpinner.hpp"
#include "../graphics/OsuColor.hpp"
#include "../../utils.hpp"
#include "../../framework/graphics/sprites/FontAwesome.hpp"

class Playground final : public Screen {
public:
    bool init() {
        if (!Screen::init()) return false;
        title("Widgets playground");
        addChild(SettingsButton::create("me", OsuColor::PinkDark));

        auto spinner = LoadingSpinner::create(true, false);
        spinner->setPosition({50,150});
        spinner->setContentSize({50,50});
        addChild(spinner);
        spinner->show();
        
        CCSprite* spinnerGlyph = FontAwesome::Solid::CircleNotch;
        spinnerGlyph->setColor({255,0,0});
        spinnerGlyph->setPosition({250,100});
        addChild(spinnerGlyph);
        
        return true;
    }
    default_create(Playground);
};
