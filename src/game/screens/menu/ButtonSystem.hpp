#pragma once
#include <Geode/Geode.hpp>
#include "OsuLogo.hpp"
#include "MainMenuButton.hpp"
#include "../../../frameworks/graphics/containers/Container.hpp"
#include "ButtonArea.hpp"
using namespace geode::prelude;

GDL_NS_START
class ButtonSystem final : public frameworks::Container
{
public:
  static float WEDGE_WIDTH;

  //Action<ButtonSystemState>? StateChanged;

private:
  OsuLogo* logo;

public:
  ButtonArea* area;
  /// <summary>
  /// Assign the <see cref="OsuLogo"/> that this ButtonSystem should manage the position of.
  /// </summary>
  /// <param name="logo">The instance of the logo to be assigned. If null, we are suspending from the screen that uses this ButtonSystem.</param>
  void setOsuLogo(OsuLogo* logo); 

  static ButtonSystem* create(OsuLogo* logo) {
    $create_class(ButtonSystem, init, logo);
  }
  bool init(OsuLogo* logo);

private:
  MenuLayer* m_menuLayerPtr;
  CreatorLayer* m_creatorLayerPtr;
  //void resize(ReactiveNode* calledNode, Property props);

  /*priv read*/ MainMenuButton* backButton;


  const char* sampleBackToLogo = "back-to-logo.wav"_spr;
  const char* sampleLogoSwoosh = "osu-logo-swoosh.wav"_spr;

};
GDL_NS_END