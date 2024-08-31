#pragma once

#include "../../graphics/containers/OsuClickableContainer.hpp"
#include "../../graphics/OsuIcon.hpp"

class ToolbarButton : public OsuClickableContainer {
    CCLabelBMFont* iconSprite;
    Container* bg;
    Container* hbg;
protected:
    Vector4 m_padding = {3};
public:
    std::string sfx = "toolbar-select.wav";
    bool init(IconConstructor icon, std::string text, std::string sub);
    void onMouseEnter() override;
    void onMouseExit() override;
    void onMouseDown(MouseEvent* e) override;
    void onMouseUp(MouseEvent* e) override;
    /**
     * @param icon Toolbar icon
     * @param text Toolbar text
     * @param sub scribe
     */
    static ToolbarButton* create(IconConstructor icon, std::string text, std::string sub) {
        create_class(ToolbarButton, init, icon, text, sub);
    }
};
