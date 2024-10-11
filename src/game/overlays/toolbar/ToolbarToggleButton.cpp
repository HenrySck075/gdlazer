#include "ToolbarToggleButton.hpp"
#include "../../graphics/OsuColor.hpp"

bool ToolbarToggleButton::init(IconConstructor icon, std::string text, std::string sub, AxisAlignment align) {
    ToolbarButton::init(icon, text, sub, align);
    toggleBg = Container::create();
    toggleBg->setColor(OsuColor::Carmine.opacity(180));
    toggleBg->setContentSizeWithUnit(CCSize(100,100), Unit::Percent, Unit::Percent);
    toggleBg->setOpacity(0);
    bgWrapper->addChild(toggleBg,-4);
    return true;
}

void ToolbarToggleButton::onClick(MouseEvent* event) {
    ToolbarButton::onClick(event);
    if (toggled) {
        deselect();
    }
    else {
        select();
    }
}
