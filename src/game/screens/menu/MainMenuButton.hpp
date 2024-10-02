#pragma once

#include <Geode/Geode.hpp>
#include "ButtonSystemState.hpp"
//#include "../../graphics/containers/BeatSyncedContainer.hpp"
#include "../../../framework/graphics/Color4.hpp"
#include "../../../framework/graphics/containers/ClickableContainer.hpp"
#include "../../../utils.hpp"

using namespace geode::prelude;

/// <summary>
/// Button designed specifically for the osu!lazer main menu.
/// </summary>
class MainMenuButton : public ClickableContainer {
    Color4 m_color = Color4(0,0,0,0);
    float const BOUNCE_COMPRESSION = 0.9f;
    float const HOVER_SCALE = 1.2f;
    float const BOUNCE_ROTATION = 8;
    bool m_askForUpdate = false;

    CCScale9Sprite* background;
    CCScale9Sprite* hover;
/*
    public readonly Key[] TriggerKeys;

    protected override Container<Drawable> Content => content;
    private readonly Container content;
    */

    const char* label;
    CCSprite* icon;

    /// <summary>
    /// The menu state for which we are visible for (assuming only one).
    /// </summary>
public:
    /*
    ButtonSystemState VisibleState
    {
        set
        {
            VisibleStateMin = value;
            VisibleStateMax = value;
        }
    }
    */
    /*
    public new MarginPadding Padding
    {
        get => Content.Padding;
        set => Content.Padding = value;
    }
    */
   void askForUpdate(bool e) {m_askForUpdate = e;}

protected:
    CCSize BaseSize = CCSize(0,0);//CCSize(ButtonSystem.BUTTON_WIDTH, ButtonArea.BUTTON_AREA_HEIGHT);


private:
    void onMouseEnter() override;
    void onMouseExit() override;
    void onMouseDown(MouseEvent* event) override;
    void onMouseUp(MouseEvent* event) override;
    void onClick(MouseEvent* e) override;

    CCSize initialSize = CCSize(0,0);//baseSize + content->getContentSize();

    const char* sampleHover = "button-hover.wav"_spr;
    // list of key that causes the click event
    std::vector<enumKeyCodes> activationKeys = {};
    /*
    public override bool IsPresent => base.IsPresent
                                        // Allow keyboard interaction based on state rather than waiting for delayed animations.
                                        || state == ButtonState.Expanded;

    public override bool ReceivePositionalInputAt(Vector2 screenSpacePos) => background.ReceivePositionalInputAt(screenSpacePos);
*/
public:
    static MainMenuButton* create(std::string text, std::string sampleClick, CCLabelBMFont* symbol, Color4 color, ButtonCallback clickAction, std::vector<enumKeyCodes> activationKeys = {}) {
        create_class(MainMenuButton, init, text, sampleClick, symbol, color, clickAction, activationKeys);
    };
    bool init(std::string text, std::string sampleClick, CCLabelBMFont* symbol, Color4 color, ButtonCallback clickAction, std::vector<enumKeyCodes> activationKeys);
    void setContentSize(const CCSize& s) override;
};
