#pragma once

#include <Geode/Geode.hpp>
#include "ButtonSystem.hpp"
using namespace geode::prelude;
using MenuSideFlashes = CCLayer*;

class MainMenu : public CCLayer {
public:
  float FADE_IN_DURATION = 300;

  float FADE_OUT_DURATION = 400;

  bool hideOverlaysOnEnter = buttons == nullptr;// || Buttons.State == ButtonSystemState.Initial;

private:
  CCLayer* songSelect;

  MenuSideFlashes sideFlashes;

protected:
  ButtonSystem buttons;
};
