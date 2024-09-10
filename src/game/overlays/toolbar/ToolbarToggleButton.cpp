#include "ToolbarToggleButton.hpp"

bool ToolbarToggleButton::init(IconConstructor icon, std::string text, std::string sub, ccColor3B stateColor) {
    ToolbarButton::init(icon, text, sub);
    toggleBg = Container::create();
    toggleBg->setColor(stateColor);
    toggleBg->setContentSizeWithUnit(CCSize(100,100), Unit::Percent, Unit::Percent);
    toggleBg->setOpacity(0);
    bgWrapper->addChild(toggleBg,-4);
    return true;
}

void ToolbarToggleButton::onClick(MouseEvent* event) {
    ToolbarButton::onClick(event);
    if (toggled) {
        toggleBg->setOpacity(0);
        deselect();
    }
    else {
        toggleBg->setOpacity(255);
        select();
    }

    toggled = !toggled;
}