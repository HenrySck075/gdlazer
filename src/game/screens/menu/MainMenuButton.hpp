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
class MainMenuButton : public BeatSyncedContainer, public ButtonBase
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
    ButtonCallback clickAction;

    CCNode* background;
    CCNode* boxHoverLayer; // ?

    CCSize initialSize = CCSize(0,0);//baseSize + content->getContentSize();

    std::string sampleName;
    char* sampleClick;
    const char* sampleHover = "j.wav";

    /*
    public override bool IsPresent => base.IsPresent
                                      // Allow keyboard interaction based on state rather than waiting for delayed animations.
                                      || state == ButtonState.Expanded;

    public override bool ReceivePositionalInputAt(Vector2 screenSpacePos) => background.ReceivePositionalInputAt(screenSpacePos);
*/
    static MainMenuButton* create(std::string text, std::string sampleName, CCSprite* symbol, Color4 color, ButtonCallback clickAction) {
      MainMenuButton *ret = new MainMenuButton();
      if (ret && ret->init(text, sampleName, symbol, color, clickAction)) {
        ret->ButtonBase::autorelease();
      } else {
        do {
          if (ret) {
            delete (ret);
            (ret) = 0;
          }
        } while (0);
      }
      return ret;
    };
    bool init(std::string text, std::string sampleName, CCSprite* symbol, Color4 color, ButtonCallback clickAction);
};
