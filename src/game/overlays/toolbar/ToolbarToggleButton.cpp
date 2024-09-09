#include "ToolbarToggleButton.hpp"

bool ToolbarToggleButton::init(IconConstructor icon, std::string text, std::string sub, ccColor3B stateColor) {
    ToolbarButton::init(icon, text, sub);
    toggleBg = Container::create();
    toggleBg->setColor(stateColor);
    toggleBg->setPadding(Vector4(3));
    toggleBg->setContentSizeWithUnit(CCSize(100,100), Unit::Percent, Unit::Percent);
    toggleBg->setOpacity(0);
    return true;
}

void ToolbarToggleButton::onClick(MouseEvent* event) {
    ToolbarButton::onClick(event);
    if (toggled) deselect();
    else select();

    toggled = !toggled;
}