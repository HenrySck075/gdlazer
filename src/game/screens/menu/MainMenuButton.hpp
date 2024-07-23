#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "ButtonSystemState.hpp"
#include "../../graphics/containers/BeatSyncedContainer.hpp"
#include "../../../framework/graphics/Color4.hpp"
#include "../../graphics/ui/ButtonBase.hpp"

using namespace cocos2d;

/// <summary>
/// Button designed specifically for the osu!next main menu.
/// In order to correctly flow, we have to use a negative margin on the parent container (due to the parallelogram shape).
/// </summary>
class MainMenuButton : public ButtonBase
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
    static MainMenuButton* create(std::string text, std::string sampleClick, CCSprite* symbol, Color4 color, ButtonCallback clickAction) {
        create_class(MainMenuButton, init, text, sampleClick, symbol, color, clickAction);
    };
    bool init(std::string text, std::string sampleClick, CCSprite* symbol, Color4 color, ButtonCallback clickAction);
    void setContentSize(const CCSize& s) override;
};
