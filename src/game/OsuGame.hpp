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
  frameworks::CCResizableSprite* m_cursor;
  frameworks::CCResizableSprite* m_cursorAdditive;
  CCNode* m_cursorNode;
  FMOD::Sound* m_clickSound;// have to manually do it

  /// Because Game is not a Container, we'd have to recreate the "drag" ourselves
  std::optional<CCPoint> m_dragStartPosition;
  bool m_dragRotating = false;

  void setMainContainerHeight(float height);
  bool doDEMidhook(frameworks::Event* event, std::type_index type) override;

  void playTapSample(float baseFreq = 1);
};
GDL_NS_END