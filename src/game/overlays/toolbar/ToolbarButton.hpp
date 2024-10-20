#pragma once

#include "../../../framework/graphics/containers/ClickableContainer.hpp"
#include "../../graphics/OsuIcon.hpp"
#include "../../../framework/graphics/containers/FillFlowContainer.hpp"

class ToolbarButton : public ClickableContainer {
    CCSprite* iconSprite;
    Container* bg;
    Container* flashBg;

    CCLayerRGBA* tooltipContainer; 
    CCLabelTTF* text;
    CCLabelTTF* subtext;

protected:
    CCClippingNode* bgWrapper;
public:
    bool init(IconConstructor icon, std::string text, std::string sub, AxisAlignment align = AxisAlignment::Start);
    void onMouseEnter() override;
    void onMouseExit() override;
    void setTooltipAlignment(AxisAlignment align) {
        static_cast<RowLayout*>(tooltipContainer->getLayout())->setAxisAlignment(align);
        switch (align) {
            case AxisAlignment::Start: 
                tooltipContainer->setAnchorPoint({0,1});
                break;
            case AxisAlignment::End: 
                tooltipContainer->setAnchorPoint({1,1});
                break;
            default: log::info("[ToolbarButton]: not supported :(");
        }
        tooltipContainer->updateLayout();
    }
    void onClick(MouseEvent* e) override;
    /**
     * @param icon Toolbar icon
     * @param text Toolbar text
     * @param sub scribe
     */
    static ToolbarButton* create(IconConstructor icon, std::string text, std::string sub, AxisAlignment align = AxisAlignment::Start) {
        create_class(ToolbarButton, init, icon, text, sub, align);
    }
};
