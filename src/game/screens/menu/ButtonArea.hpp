#include "Geode/cocos/actions/CCActionInterval.h"
#include <Geode/Geode.hpp>
#include "ButtonConstants.hpp"
using namespace cocos2d;

class ButtonArea : public CCNodeRGBA {
  protected:
    CCNode* content;

  private:
    CCNodeRGBA* buttonAreaBackground;

  public:

    bool init()
    {
      buttonAreaBackground = CCNodeRGBA::create();
#define button buttonAreaBackground
      button->setAnchorPoint(ccp(0.5,0.5));
      auto ws = CCDirector::sharedDirector()->getWinSize();
      button->setPosition(ws/2);
      button->setContentSize(CCSize(ws.width, BUTTON_AREA_HEIGHT));
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
    void hide() {
      buttonAreaBackground->runAction(CCSpawn::createWithTwoActions(
        CCFadeOut::create(0.3),
        CCScaleTo::create(0.3,1,0)
      ));
    };

    void show() {
      buttonAreaBackground->runAction(CCSpawn::createWithTwoActions(
        CCFadeIn::create(0.3),
        CCScaleTo::create(0.3,1,1)
      ));
    };
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
enum ButtonAreaBackgroundState
{
    Normal,
    Flat
};
