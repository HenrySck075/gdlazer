#pragma once

#include "../frameworks/Game.hpp"
#include "../frameworks/graphics/sprites/CCResizableSprite.hpp"
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
  geode::Ref<frameworks::CCResizableSprite> m_cursor;
  void setMainContainerHeight(float height);
  bool doDEMidhook(frameworks::Event* event, std::type_index type) override;
};
GDL_NS_END