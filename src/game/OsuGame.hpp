#pragma once

#include "../frameworks/Game.hpp"
#include "overlays/toolbar/Toolbar.hpp"

GDL_NS_START
class OsuGame : public frameworks::Game {
public:
  bool init() override;

  static geode::Ref<OsuGame> get(bool createIfNotExist = true);

  void showToolbar();
  void hideToolbar();

  bool doDispatchEvent(frameworks::Event* event, std::type_index type) override;
private:
  geode::Ref<Toolbar> m_toolbar;
  void setScreensContainerHeight(float height);
};
GDL_NS_END