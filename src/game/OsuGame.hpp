#pragma once

#include "../frameworks/Game.hpp"

GDL_NS_START
class OsuGame : public GDF_NS::Game {
public:
  static geode::Ref<OsuGame> get(bool createIfNotExist = true);
};
GDL_NS_END