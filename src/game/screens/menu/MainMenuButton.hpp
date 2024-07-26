#pragma once

#include <Geode/Geode.hpp>
#include "ButtonSystemState.hpp"
#include "../../graphics/containers/BeatSyncedContainer.hpp"
#include "../../../framework/graphics/Color4.hpp"
#include "../../graphics/ui/ButtonBase.hpp"
#include <henrysck075.cocosutils/include/ReactiveNode.hpp>

using namespace geode::prelude;

/// <summary>
/// Button designed specifically for the osu!lazer main menu.
/// </summary>
class MainMenuButton : public ButtonBase, public ReactiveNode
{
    float const BOUNCE_COMPRESSION = 0.9f;
    float const HOVER_SCALE = 1.2f;
    float const BOUNCE_ROTATION = 8;
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

protected:
    CCSize BaseSize = CCSize(0,0);//CCSize(ButtonSystem.BUTTON_WIDTH, ButtonArea.BUTTON_AREA_HEIGHT);


private:
    void onMouseEnter() override;
    void onMouseExit() override;
    ButtonCallback clickAction;

    CCNode* background;
    CCNode* boxHoverLayer; // ?

    CCSize initialSize = CCSize(0,0);//baseSize + content->getContentSize();

    const char* sampleHover = "button-hover.wav"_spr;

    /*
    public override bool IsPresent => base.IsPresent
                                        // Allow keyboard interaction based on state rather than waiting for delayed animations.
                                        || state == ButtonState.Expanded;

    public override bool ReceivePositionalInputAt(Vector2 screenSpacePos) => background.ReceivePositionalInputAt(screenSpacePos);
*/
public:
    static MainMenuButton* create(gd::string text, gd::string sampleClick, CCLabelBMFont* symbol, Color4 color, ButtonCallback clickAction) {
        create_class(MainMenuButton, init, text, sampleClick, symbol, color, clickAction);
    };
    bool init(gd::string text, gd::string sampleClick, CCLabelBMFont* symbol, Color4 color, ButtonCallback clickAction);
    void setContentSize(const CCSize& s) override;
};
