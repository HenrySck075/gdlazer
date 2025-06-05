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
  static float s_wedgeWidth;

  //Action<ButtonSystemState>? StateChanged;

private:
  Ref<OsuLogo> m_logo;

public:
  Ref<ButtonArea> m_area;
  /// <summary>
  /// Assign the <see cref="OsuLogo"/> that this ButtonSystem should manage the position of.
  /// </summary>
  /// <param name="logo">The instance of the logo to be assigned. If null, we are suspending from the screen that uses this ButtonSystem.</param>
  void setOsuLogo(OsuLogo* logo); 

  static ButtonSystem* create(OsuLogo* logo) {
    $createClass(ButtonSystem, init, logo);
  }
  bool init(OsuLogo* logo);

private:
  Ref<MenuLayer> m_menuLayerPtr;
  Ref<CreatorLayer> m_creatorLayerPtr;
  //void resize(ReactiveNode* calledNode, Property props);

  Ref<MainMenuButton> m_backButton;


  const char* sampleBackToLogo = "back-to-logo.wav"_spr;
  const char* sampleLogoSwoosh = "osu-logo-swoosh.wav"_spr;

};
GDL_NS_END
