#pragma once

#include <Geode/Geode.hpp>
//#include "ButtonSystem.hpp"
//#include "../../graphics/containers/beatsync/BeatDetector.hpp"
#include "Background.hpp"
#include "ButtonSystem.hpp"
#include "../../../framework/screens/Screen.hpp"
class OsuGame;
#include "../../OsuGame.hpp"
#include "../../../utils.hpp"
#include "../../graphics/containers/ParallaxContainer.hpp"
using namespace geode::prelude;
using MenuSideFlashes = CCLayer;

class MainMenu final : public Screen {
  ButtonSystem* buttonSys;
  Background* bg;
  ParallaxContainer* buttonSysParallax;
  CCMoveToModifiable* logoMoveAction;

  CCLayerRGBA* joe;
public:
  float FADE_IN_DURATION = 300;

  float FADE_OUT_DURATION = 400;

  bool hideOverlaysOnEnter = false;//buttons == nullptr;// || Buttons.State == ButtonSystemState.Initial;

  static MainMenu* create();
  bool init();

  // open the menu
  void onLogoClickIdle();
private:
  void debugReturn(CCObject *t);
  void onExiting(ScreenTransitionEvent e) override {
    // setVisible(false);
    if (e.Destination == nullptr) removeFromParent();
    else {
      setCascadeOpacityEnabled(true);
      bg->runAction(CCFadeOut::create(2));
      runAction(CCSequence::createWithTwoActions(
        CCDelayTime::create(2),
        CCFadeOut::create(1)
      ));
      buttonSys->area->hide(
        buttonSys->area->getCurrent().value(), true, true
      );
    }
  }
  //BeatDetector* detector;

  CCLayer* songSelect;

  MenuSideFlashes sideFlashes;

protected:
  int idkTho = 0;
  //ButtonSystem buttons;
};
