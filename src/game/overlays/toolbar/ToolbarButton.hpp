#pragma once

#include "../../graphics/containers/OsuClickableContainer.hpp"
#include "../../graphics/OsuIcon.hpp"
#include "../../../framework/graphics/containers/FillFlowContainer.hpp"

class ToolbarButton : public OsuClickableContainer {
    CCLabelBMFont* iconSprite;
    Container* bg;
    Container* flashBg;

    CCLayerRGBA* tooltipContainer; 
    CCLabelTTF* text;
    CCLabelTTF* subtext;
protected:
    CCClippingNode* bgWrapper;
public:
    bool init(IconConstructor icon, std::string text, std::string sub);
    void onMouseEnter() override;
    void onMouseExit() override;
    void onClick(MouseEvent* e) override;
    /**
     * @param icon Toolbar icon
     * @param text Toolbar text
     * @param sub scribe
     */
    static ToolbarButton* create(IconConstructor icon, std::string text, std::string sub) {
        create_class(ToolbarButton, init, icon, text, sub);
    }
};
