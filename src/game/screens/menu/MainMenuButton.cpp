#include "MainMenuButton.hpp"

bool MainMenuButton::init(std::string text, std::string sampleName,
                          IconUsage symbol, Color4 colour,
                          SEL_MenuHandler clickAction) {
  this->sampleName = sampleName;
  this->clickAction = clickAction;
  // TriggerKeys = triggerKeys;
  //
  this->setOpacity(0);

  background = CCNode::create();
  background->setContentSize(BaseSize);
  background->setAnchorPoint(ccp(0.5, 0.5));

  /*
          Children = new[]
          {
              backgroundContent = CreateBackground(colour).With(bg =>
              {
                  bg.RelativeSizeAxes = Axes.Y;
                  bg.Anchor = Anchor.Centre;
                  bg.Origin = Anchor.Centre;
              }),
              boxHoverLayer = new Box
              {
                  EdgeSmoothness = new Vector2(1.5f, 0),
                  RelativeSizeAxes = Axes.Both,
                  Blending = BlendingParameters.Additive,
                  Colour = Color4.White,
                  Depth = float.MinValue,
                  Alpha = 0,
              },
          }
      },
      content = new Container
      {
          RelativeSizeAxes = Axes.Both,
          Anchor = Anchor.Centre,
          Origin = Anchor.Centre,
          Children = new Drawable[]
          {
              new OsuSpriteText
              {
                  Shadow = true,
                  AllowMultiline = false,
                  Anchor = Anchor.BottomCentre,
                  Origin = Anchor.BottomCentre,
                  Margin = new MarginPadding
                  {
                      Left = -3,
                      Bottom = 7,
                  },
                  Text = text
              },
              icon = new SpriteIcon
              {
                  Shadow = true,
                  Anchor = Anchor.Centre,
                  Origin = Anchor.Centre,
                  Size = new Vector2(32),
                  Position = new Vector2(0, 0),
                  Margin = new MarginPadding { Top = -4 },
                  Icon = symbol
              }
          }
      }
  });
  */
  return true;
}

