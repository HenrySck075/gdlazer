#pragma once

#include "../frameworks/Game.hpp"

GDL_NS_START
class OsuGame : public frameworks::Game {
public:
  static geode::Ref<OsuGame> get(bool createIfNotExist = true);

  void showToolbar() {};
  void hideToolbar() {};
};
GDL_NS_END