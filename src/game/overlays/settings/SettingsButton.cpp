#include "SettingsButton.hpp"


bool SettingsButton::init(std::string label, ccColor3B color) {
    if (!ClickableContainer::init("button-select.wav"_spr)) return false;
    setColor(color);
    setOpacity(255);
    //setRadius(8);
    setContentSizeWithUnit({100,40}, Unit::Percent, Unit::UIKit);
    return true;
}
