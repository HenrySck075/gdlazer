#pragma once

#include <Geode/Geode.hpp>
#include "ButtonSystemState.hpp"
//#include "../../graphics/containers/BeatSyncedContainer.hpp"
#include "../../../frameworks/graphics/color/Color4.hpp"
#include "../../../frameworks/graphics/containers/ClickableContainer.hpp"
#include "../../../frameworks/graphics/sprites/CCResizableSprite.hpp"
#include "../../../frameworks/graphics/sprites/IconConstructor.hpp"
#include "../../../utils.hpp"

using namespace geode::prelude;

GDL_NS_START
/// <summary>
/// Button designed specifically for the osu!lazer main menu.
/// </summary>
class MainMenuButton : public frameworks::ClickableContainer {
public:
  /// <summary>
  /// The menu state for which we are visible for (assuming only one).
  /// </summary>
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

  static MainMenuButton* create(std::string text, std::string sampleClick, IconConstructor symbol, Color4 color, ButtonCallback clickAction, std::vector<enumKeyCodes> activationKeys = {}) {
    $create_class(MainMenuButton, init, text, sampleClick, symbol, color, clickAction, activationKeys);
  };
  bool init(std::string text, std::string sampleClick, IconConstructor symbol, Color4 color, ButtonCallback clickAction, std::vector<enumKeyCodes> activationKeys);
  void setContentSize(const CCSize& s) override;
  void setOpacity(GLubyte opacity) override {
    GLubyte old = hover->getOpacity();
    Container::setOpacity(opacity);
    hover->setOpacity(old);
  }
  
protected:
  CCSize BaseSize = CCSize(0,0);//CCSize(ButtonSystem.BUTTON_WIDTH, ButtonArea.BUTTON_AREA_HEIGHT);

private:
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
  CCSize initialSize = CCSize(0,0);//baseSize + content->getContentSize();

  const char* sampleHover = "button-hover.wav"_spr;
  // list of key that causes the click event
  std::vector<enumKeyCodes> activationKeys = {};
};
GDL_NS_END