#include "Geode/cocos/cocoa/CCGeometry.h"
#include <Geode/Geode.hpp>
using namespace cocos2d;

class ButtonArea : public CCNodeRGBA {
  protected:
    CCNode* content;

  private:
    CCNodeRGBA* buttonAreaBackground;

  public:
    float BUTTON_AREA_HEIGHT = 100;

    bool init()
    {
      buttonAreaBackground = CCNodeRGBA::create();
#define button buttonAreaBackground
      button->setAnchorPoint(ccp(0.5,0.5));
      auto ws = CCDirector::sharedDirector()->getWinSize();
      button->setPosition(ws/2);
      button->setContentSize(CCSizeMake(ws.width, BUTTON_AREA_HEIGHT));
      button->setOpacity(0);

      this->addChild(buttonAreaBackground);

      return true;
      /*
        RelativeSizeAxes = Axes.Both;
        InternalChild = new Container
        {
            Anchor = Anchor.Centre,
            Origin = Anchor.Centre,
            RelativeSizeAxes = Axes.X,
            Size = new Vector2(1, BUTTON_AREA_HEIGHT),
            Alpha = 0,
            AlwaysPresent = true, // Always needs to be present for correct tracking on initial -> toplevel state change
            Children = new Drawable[]
            {
                buttonAreaBackground = new ButtonAreaBackground(),
                Flow = new FlowContainerWithOrigin
                {
                    Direction = FillDirection.Horizontal,
                    Spacing = new Vector2(-ButtonSystem.WEDGE_WIDTH, 0),
                    Anchor = Anchor.Centre,
                    AutoSizeAxes = Axes.Both,
                }
            }
        };
        */
    }
    void hide() => State = Visibility.Hidden;

    void show() => State = Visibility.Visible;
};
/*
    public ButtonSystemState ButtonSystemState
    {
        set
        {
            switch (value)
            {
                case ButtonSystemState.Exit:
                case ButtonSystemState.Initial:
                case ButtonSystemState.EnteringMode:
                    Hide();
                    break;

                case ButtonSystemState.TopLevel:
                case ButtonSystemState.Play:
                    Show();
                    break;
            }

            buttonAreaBackground.ButtonSystemState = value;
        }
    }
    */

    public Visibility State
    {
        get => state;
        set
        {
            if (value == state) return;

            state = value;
            InternalChild.FadeTo(state == Visibility.Hidden ? 0 : 1, 300);
            StateChanged?.Invoke(state);
        }
    }

private partial class ButtonAreaBackground : Box, IStateful<ButtonAreaBackgroundState>
{
    private ButtonAreaBackgroundState state;

    public ButtonAreaBackground()
    {
        RelativeSizeAxes = Axes.Both;
        Size = new Vector2(2, 1);
        Colour = OsuColour.Gray(50);
        Anchor = Anchor.Centre;
        Origin = Anchor.Centre;
    }

    public ButtonAreaBackgroundState State
    {
        get => state;
        set
        {
            if (value == state) return;

            state = value;

            switch (state)
            {
                case ButtonAreaBackgroundState.Flat:
                    this.ScaleTo(new Vector2(2, 0), 300, Easing.InSine);
                    break;

                case ButtonAreaBackgroundState.Normal:
                    this.ScaleTo(Vector2.One, 400, Easing.OutQuint);
                    break;
            }

            StateChanged?.Invoke(state);
        }
    }

    public ButtonSystemState ButtonSystemState
    {
        set
        {
            switch (value)
            {
                default:
                    State = ButtonAreaBackgroundState.Normal;
                    break;

                case ButtonSystemState.Initial:
                case ButtonSystemState.Exit:
                case ButtonSystemState.EnteringMode:
                    State = ButtonAreaBackgroundState.Flat;
                    break;
            }
        }
    }

    [CanBeNull]
    public event Action<ButtonAreaBackgroundState> StateChanged;
}

public enum ButtonAreaBackgroundState
{
    Normal,
    Flat
}
