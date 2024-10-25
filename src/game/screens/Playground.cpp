#include "Playground.hpp"
bool Playground::init() {
    if (!Screen::init()) return false;
    setColor({255,255,255,255});
    setOpacity(255);
    // you fucking maniacs
    setCascadeColorEnabled(false);
    setCascadeOpacityEnabled(false);
    title("Widgets playground");
    addChild(SettingsButton::create("me", OsuColor::PinkDark));

    auto spinner = LoadingSpinner::create(true, true);
    spinner->setPosition({50,150});
    spinner->setContentSize({50,50});
    addChild(spinner);
    spinner->show();
    
    CCResizableSprite* spinnerGlyph = FontAwesome::Solid::CircleNotch;
    spinnerGlyph->setColor({255,0,0});
    spinnerGlyph->setPosition({250,100});
    addChild(spinnerGlyph);
    
    auto roundRect = balls::create(8);
    addChild(roundRect);
    roundRect->setPosition({300,150});
    roundRect->setContentSize({150,100});

    return true;
}
