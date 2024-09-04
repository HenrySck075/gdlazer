/// @note ToolbarOverlayToggleButton

#pragma once

#include "ToolbarButton.hpp"

class ToolbarToggleButton : public ToolbarButton {
    Container* toggleBg;
    bool toggled = false;
public:
    bool init(IconConstructor icon, std::string text, std::string sub, ccColor3B stateColor);
    static ToolbarToggleButton* create(IconConstructor icon, std::string text, std::string sub, ccColor3B stateColor) {
        create_class(ToolbarToggleButton, init, icon, text, sub, stateColor);
    };
    void onClick(MouseEvent* e) override;

    virtual void select() {
        toggleBg->setOpacity(255);
    }
    virtual void deselect() {
        toggleBg->setOpacity(0);
    }
};
