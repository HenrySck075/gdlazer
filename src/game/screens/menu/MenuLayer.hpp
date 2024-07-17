#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using MenuSideFlashes = CCLayer*;
using ButtonSystem = CCLayer*;

class MainMenu : public CCLayer {
public:
  float FADE_IN_DURATION = 300;

  float FADE_OUT_DURATION = 400;

  bool hideOverlaysOnEnter = buttons == nullptr;// || Buttons.State == ButtonSystemState.Initial;

  // keep this here
  bool allowBackButton = false;

  bool allowExternalScreenChange = true;

  bool allowGlobalTrackControl = true;

private:
  CCLayer* songSelect;

  MenuSideFlashes sideFlashes;

protected:
  ButtonSystem buttons;
};
