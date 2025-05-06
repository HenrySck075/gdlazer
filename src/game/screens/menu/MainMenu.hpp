#pragma once

#include <Geode/Geode.hpp>
//#include "ButtonSystem.hpp"
//#include "../../graphics/containers/beatsync/BeatDetector.hpp"
#include "Background.hpp"
#include "ButtonSystem.hpp"
#include "../../../frameworks/screens/Screen.hpp"
class OsuGame;
#include "../../OsuGame.hpp"
#include "../../../utils.hpp"
#include "../../graphics/containers/ParallaxContainer.hpp"
using namespace geode::prelude;
using MenuSideFlashes = CCLayer;

GDL_NS_START
class MainMenu final : public frameworks::Screen {
  Ref<ButtonSystem> m_buttonSys;
  Ref<Background> m_background;
  Ref<ParallaxContainer> m_buttonSysParallax;
  Ref<CCMoveToModifiable> m_logoMoveAction;

  Ref<CCLayerRGBA> m_nowPlayingBox;
public:
  float FADE_IN_DURATION = 300;

  float FADE_OUT_DURATION = 400;

  bool hideOverlaysOnEnter = false;//buttons == nullptr;// || Buttons.State == ButtonSystemState.Initial;

  static MainMenu* create();
  bool init();

  // open the menu
  void onLogoClickIdle();
private:
  void onScreenExit(frameworks::ScreenTransitionEvent e) override;
  //BeatDetector* detector;

  CCLayer* songSelect;

  MenuSideFlashes sideFlashes;

protected:
  int idkTho = 0;
  //ButtonSystem buttons;
};
GDL_NS_END