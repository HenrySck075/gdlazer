#include "OsuGame.hpp"
#include "overlays/toolbar/ToolbarConstants.hpp"
#include "../frameworks/graphics/animations/ActionEase.hpp"
#include "../frameworks/graphics/color/Color4.hpp"
#include "../frameworks/audio/AudioManager.hpp"
#include "../helpers/CustomActions.hpp"
#include <random>

namespace {
  std::mutex g_fish;
};

#include <Geode/modify/LevelTools.hpp>
struct LevelToolsCustomSong : geode::Modify<LevelToolsCustomSong, LevelTools> {
  static gd::string getAudioTitle(int trackID) {
    switch (trackID) {
      case -7: return "Triangles";
      case -8: return "circles!";
      default: return LevelTools::getAudioTitle(trackID);
    }
  }
  static int artistForAudio(int trackID) {
    switch (trackID) {
      case -7: return -1;
      case -8: return -2;
      default: return LevelTools::artistForAudio(trackID);
    }
  }
  static gd::string getAudioFileName(int trackID) {
    switch (trackID) {
      case -7: return "triangles.mp3"_spr;
      case -8: return "circles.mp3"_spr;
      default: return LevelTools::getAudioFileName(trackID);
    }
  }
  static gd::string nameForArtist(int artistID) {
    switch (artistID) {
      case -1: return "cYsmix";
      case -2: return "nekodex";
      default: return LevelTools::nameForArtist(artistID);
    }
  }
};

GDL_NS_START
using namespace frameworks;
geode::Ref<OsuGame> OsuGame::get(bool create) {
  auto s_instance = Game::get();
  if ((!s_instance || geode::cast::typeinfo_cast<OsuGame*>(s_instance.operator->()) == nullptr) && create) {
    g_fish.lock();
    s_instance = new OsuGame();
    if (s_instance->init()) {
      s_instance->autorelease();
      setInstance(s_instance);
    } else {
      delete s_instance;
      s_instance = nullptr;
    }
    g_fish.unlock();
  }
  return static_cast<OsuGame*>(s_instance.operator->());
};
void OsuGame::setMouseVisibility(bool visible) {
  m_cursorNode->setVisible(visible);
};
void OsuGame::update(float dt) {
  auto lastChild = static_cast<CCNode*>(getChildren()->lastObject());
  if (lastChild!=m_cursorNode && lastChild->getZOrder() > m_cursorNode->getZOrder()) {
    m_cursorNode->setZOrder(lastChild->getZOrder());
  }
  Game::update(dt);
};
bool OsuGame::init() {

  addListener<MouseEvent>([this](MouseEvent* cure) {
    if (cure->m_eventType == MouseEventType::Move) {
      m_cursorNode->setPosition(cure->m_position);
      if (
        cure->m_clicked && 
        m_dragStartPosition.has_value() && 
        m_dragStartPosition->getDistance(cure->m_position) > sqrtf(
          powf(m_everypence->processUnit(80, Unit::UIKit, true), 2) 
          +
          powf(m_everypence->processUnit(80, Unit::UIKit, false), 2) 
        )
      ) {
        // save the result to not recalculate again
        m_dragRotating = true;
      }

      if (m_dragRotating) {
        float d = kmRadiansToDegrees(atan2f(
          m_dragStartPosition.value().y - cure->m_position.y, 
          - (m_dragStartPosition.value().x - cure->m_position.x)
        )) + 24.3;
        // Always rotate in the direction of least distance
        float diff = fmod(d - m_cursorNode->getRotation(), 360);
        if (diff < -180) diff += 360;
        if (diff > 180) diff -= 360;
        d = m_cursorNode->getRotation() + diff;
        //m_cursorNode->stopActionByTag(9);
        //m_cursorNode->runAction(ActionEase::create(CCRotateTo::create(0.12, d), Easing::OutQuint))/*->setTag(9)*/;
        m_cursorNode->setRotation(d);
      }

    } 
    else if (cure->m_eventType == MouseEventType::MouseDown) {
      m_cursorNode->stopAllActions();
      m_cursorNode->runAction(
        ActionEase::create(
          CCScaleTo::create(0.8, 0.9), Easing::OutQuint
        )
      );
      m_cursorAdditive->runAction(
        ActionEase::create(
          CCFadeIn::create(0.8), Easing::OutQuint
        )
      );
      m_dragStartPosition = cure->m_position;
      playTapSample();
    } 
    else if (cure->m_eventType == MouseEventType::MouseUp) {
      m_cursorNode->stopAllActions();
      m_cursorNode->runAction(
        ActionEase::create(
          CCScaleTo::create(0.8, 1), Easing::OutElastic
        )
      );
      m_cursorAdditive->runAction(
        ActionEase::create(
          CCFadeOut::create(0.8), Easing::OutQuint
        )
      );
      m_dragStartPosition = std::nullopt;
      if (m_dragRotating) {
        m_cursorNode->stopAllActions();
        m_cursorNode->runAction(
          ActionEase::create(
            CCRotateTo::create(0.4 * (0.5f + abs(m_cursorNode->getRotation() / 960)), 0),
            Easing::OutElasticQuarter
          )
        );
      }
      m_dragRotating = false;
      playTapSample(0.8);
    }
    if (m_containsBlockingUIInFront) {
      cure->stopImmediatePropagation();
    }
    return !m_containsBlockingUIInFront;
  });

  if (!Game::init()) return false;

  m_toolbar = $verifyPtr(Toolbar::create());
  m_setupComplete = false;
  
  queueInMainThread([this]{
    m_setupComplete = false;
    addChild(m_toolbar, 9);
    m_setupComplete = true;
  });

#ifdef GEODE_IS_DESKTOP
  addChild(m_cursorNode = CCNode::create(), 12);
  m_cursorNode->setVisible(false);

  /// No const wchar_t allowed this will guarantee a crash on non-ascii gd install path
  FMODAudioEngine::get()->m_system->createSound((Mod::get()->getResourcesDir() / "cursor-tap.wav").string().c_str(), FMOD_DEFAULT, 0, &m_clickSound);
  
  m_cursorNode->addChild(m_cursor = CCResizableSprite::create("menu-cursor.png"_spr));
  m_cursor->setContentSize({10,442/(312/10.f)});
  m_cursor->setAnchorPoint({0,1});
  
  m_cursorNode->addChild(m_cursorAdditive = CCResizableSprite::create("menu-cursor-additive.png"_spr));
  m_cursorAdditive->setContentSize({10,442/(312/10.f)});
  m_cursorAdditive->setAnchorPoint({0,1});
  m_cursorAdditive->setOpacity(0);
  m_cursorAdditive->setCascadeColorEnabled(true);
  m_cursorAdditive->setColor(Color4::Pink);
#endif

  m_everypence->setContentSize(m_everypence->getContentSize());

  auto onlineLevels = GameLevelManager::sharedState()->m_onlineLevels;
  // 1st: song id
  std::unordered_map<int, GJGameLevel*> levelsBySongID;
  for (auto e : CCDictionaryExt<int, GJGameLevel>(onlineLevels)) {
    auto level = e.second;
    int songId = level->m_songIDs.size() != 0 ? level->m_songIDs[0] : level->m_songID;
    auto i = levelsBySongID.find(songId);
    if (i==levelsBySongID.end()) {
      levelsBySongID.insert(std::make_pair(songId, level));
    } else {
      if (i->second->m_downloads < level->m_downloads) {
        i->second = level;
      }
    }
  }
  for (auto e : levelsBySongID) m_playlist.push_back(e.second);

  m_setupComplete = true;

  return true;
}

void OsuGame::startMusicSequence() {
  m_songIndex = -1;
  startNextSong();
}
void OsuGame::startNextSong() {
  m_songIndex++;
  AudioManager::get()->playFromLevel(m_playlist[m_songIndex], 0);
}

void OsuGame::addChild(CCNode* child) {
  addChild(child, child->getZOrder());
}
void OsuGame::addChild(CCNode* child, int zOrder) {
  CCScene::addChild(child, zOrder);
  if (m_setupComplete) {
    m_containsBlockingUIInFront = true;
  }
}
void OsuGame::removeChild(CCNode* child, bool cleanup) {
  CCScene::removeChild(child, cleanup);
  // traverse children in reverse direction to check if there's still a ui-blocking elements
  // its cocos2d-x do the job right
  m_containsBlockingUIInFront = false;
  if (auto children = getChildren()) {
    int length = children->count();
    int mainContainerIndex = children->indexOfObject(m_screensContainer->getParent());
    for (int i = length-1; i > mainContainerIndex; --i) {
      auto c = static_cast<CCNode*>(children->objectAtIndex(i));
      if (c != m_toolbar && c != m_cursorNode) {
        m_containsBlockingUIInFront = true;
        break;
      }
    }
  }
  log::debug("im n jom {}", m_containsBlockingUIInFront);
}

void OsuGame::setWindowTitle(std::string title) {
  setWindowTitleReal(title.size() != 0 ? title+" | gd!lazer" : "gd!lazer");
}

bool OsuGame::doDispatchEvent(Event* event, std::type_index type) {
  // Relaunch on Ctrl-R
  #ifdef GDL_DEBUG
  if (type == typeid(KeyEvent)) {
    auto ke = static_cast<KeyEvent*>(event);
    if (ke->m_pressed && ke->m_key == cocos2d::KEY_R && ke->m_modifiers.ctrl) {
      geode::utils::game::restart();
    }
  }
  #endif
  /*
  if (
    m_containsBlockingUIInFront && 
    ((
      type == typeid(MouseEvent) && static_cast<MouseEvent*>(event)->m_eventType != frameworks::MouseEventType::Move
    ) || type == typeid(KeyEvent))
  ) return false;
   */
  if (!Game::doDispatchEvent(event, type)) return false;
  if (type == typeid(NodeSizeUpdated)) {
    m_everypence->setContentSize({getContentWidth(), getContentHeight()-(m_toolbar->isOpen() ? m_everypence->processUnit(ToolbarConstants::c_height, Unit::UIKit, false) : 0)});
  }
  return true;
}

bool OsuGame::doDEMidhook(Event* event, std::type_index type) {
  bool ret = m_toolbar->doDispatchEvent(event, type);

  return type == typeid(NodeSizeUpdated) || ret;
}

void OsuGame::showToolbar() {
  m_toolbar->show();
  m_everypence->runAction(ActionEase::create(
    CCCustomTween::create(
      getContentHeight(), 
      getContentHeight()-m_everypence->processUnit(
        ToolbarConstants::c_height, 
        Unit::UIKit, false
      ), 
      0.5, 
      this, customtween_selector(OsuGame::setMainContainerHeight)
    ), Easing::Out
  ));
};
void OsuGame::hideToolbar() {
  m_toolbar->hide();
  m_everypence->runAction(ActionEase::create(
    CCCustomTween::create(
      getContentHeight()-m_everypence->processUnit(
        ToolbarConstants::c_height, 
        Unit::UIKit, false
      ), 
      getContentHeight(), 
      0.5, 
      this, customtween_selector(OsuGame::setMainContainerHeight)
    ), Easing::Out
  ));
};

void OsuGame::setMainContainerHeight(float height) {
  m_everypence->setContentSize({100, height}, Unit::Percent, Unit::OpenGL);
  m_everypence->dispatchEvent(new NodeSizeUpdated()); // why
}

/// Store a uniform random distribution out here so we don't have to recreate one on every playTapSample call
static std::mt19937 gen((std::random_device())());
static std::uniform_real_distribution<float> g_freqDist(0, 0.02f);

void OsuGame::playTapSample(float baseFreq) {
  if (!m_cursorNode->isVisible()) return;
  auto audio = FMODAudioEngine::get();
  FMOD::Channel* channel = nullptr;
  audio->m_system->playSound(m_clickSound, audio->m_globalChannel, false, &channel);
  if (channel) {
    channel->setPan(((m_cursorNode->getPositionX() / getContentWidth()) * 2 - 1) * 0.75);
    float freq;
    m_clickSound->getDefaults(&freq, 0);
    channel->setFrequency((baseFreq - (0.02 / 2.f) + g_freqDist(gen)) * freq);
    channel->setVolume(baseFreq*audio->m_sfxVolume);
    //channel->setChannelGroup(FMODAudioEngine::sharedEngine()->m_globalChannel);
  }
  //audio->playEffectAdvanced("cursor-tap.wav"_spr, baseFreq - (0.02 / 2.f) + g_freqDist(gen), 0, baseFreq, 1, false, false, 0, 0, 0, 0,false,0,true, );
}

GDL_NS_END
