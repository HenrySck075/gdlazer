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

  void addChild(CCNode* child) override;
  void addChild(CCNode* child, int zOrder) override;

  void removeChild(CCNode* child, bool cleanup) override;

  void update(float) override;

  void setMouseVisibility(bool visible);

  void startMusicSequence();
  void startNextSong();
protected:
  void setWindowTitle(std::string title) override;

private:
  bool m_containsBlockingUIInFront = false;
  bool m_setupComplete = false;
  CCArrayExt<GJGameLevel> m_playlist; friend class IntroTriangles;
  int m_songIndex = 0;
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