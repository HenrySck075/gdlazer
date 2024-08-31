#pragma once

#include "../../graphics/containers/OsuClickableContainer.hpp"
#include "../../graphics/OsuIcon.hpp"

class ToolbarButton : public OsuClickableContainer {
    CCLabelBMFont* iconSprite;
public:
    std::string sfx = "toolbar-select.wav";
    bool init(IconConstructor icon);
    void onMouseEnter() override;
    /**
     * @param icon Toolbar icon
     * @param text Toolbar text
     * @param sub scribe
     */
    static ToolbarButton* create(IconConstructor icon, std::string text, std::string sub) {
        create_class(ToolbarButton, init, icon);
    }
};
