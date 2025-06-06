#include "OsuGame.hpp"
#include "overlays/toolbar/ToolbarConstants.hpp"
#include "../frameworks/graphics/CCEase2.hpp"
#include "../frameworks/graphics/color/Color4.hpp"
#include "../helpers/CustomActions.hpp"
#include <random>

namespace {
  std::mutex g_fish;
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
bool OsuGame::init() {
  if (!Game::init()) return false;
  m_toolbar = $verifyPtr(Toolbar::create());
  addChild(m_toolbar);

#ifdef GEODE_IS_DESKTOP
  addChild(m_cursorNode = CCNode::create(), 999999);
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

  const float dragMinDistUI = m_everypence->processUnit(80, Unit::UIKit, true);

  addListener<MouseEvent>([this, dragMinDistUI](MouseEvent* cure) {
    if (cure->m_eventType == MouseEventType::Move) {
      m_cursorNode->setPosition(cure->m_position);
      if (
        cure->m_clicked && 
        m_dragStartPosition.has_value() && 
        m_dragStartPosition->getDistance(cure->m_position) > dragMinDistUI
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
      m_dragStartPosition = std::make_optional(cure->m_position);
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
    return true;
  });
#endif
  m_everypence->setContentSize(m_everypence->getContentSize());

  return true;
}

bool OsuGame::doDispatchEvent(Event* event, std::type_index type) {
  /// Relaunch on Ctrl-R
  #ifdef GDL_DEBUG
  if (type == typeid(KeyEvent)) {
    auto ke = static_cast<KeyEvent*>(event);
    if (ke->m_pressed && ke->m_key == cocos2d::KEY_R && ke->m_modifiers.ctrl) {
      geode::utils::game::restart();
    }
  }
  #endif
  if (!Game::doDispatchEvent(event, type)) return false;
  if (type == typeid(NodeLayoutUpdated)) {
    m_everypence->setContentSize({getContentWidth(), getContentHeight()-(m_toolbar->isOpen() ? m_everypence->processUnit(ToolbarConstants::c_height, Unit::UIKit, false) : 0)});
  }
  return true;
}

bool OsuGame::doDEMidhook(Event* event, std::type_index type) {
  bool ret = m_toolbar->doDispatchEvent(event, type);

  return type == typeid(NodeLayoutUpdated) || ret;
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
  m_everypence->dispatchEvent(new NodeLayoutUpdated(m_everypence)); // why
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
