#include "LayerCapture.hpp"
#include "../OsuGame.hpp"

using namespace geode::prelude;

#include <Geode/modify/CCTransitionScene.hpp>
struct owo : public Modify<owo, CCTransitionScene> {
  struct Fields {
    CCScene* m_inScene;
    CCScene* m_outScene;
  };
  virtual bool initWithDuration(float t,CCScene* scene) {
    if (!CCTransitionScene::initWithDuration(t, scene)) return false;
    auto fields = m_fields.self();
    fields->m_inScene = m_pInScene;
    fields->m_outScene = m_pOutScene;
    log::debug("[hook: CCTransitionScene]: in: {}, out: {}", m_pInScene, m_pOutScene);
    return true;
  };
};

static Ref<CCScene> s_requestedScene;

static bool s_replaceSceneHookEnabled = false;
static bool s_getWinSizeHookEnabled = false;

#include <Geode/modify/CCDirector.hpp>
struct theresmultipleccdirectorhooksscatteredaround 
  : public Modify<theresmultipleccdirectorhooksscatteredaround, CCDirector> {
    /*
  static void onModify(auto& self) {
    auto rs = self.getHook("cocos2d::CCDirector::replaceScene");
    if (!rs) {
      log::error("dum bass");
      return;
    }
    s_replaceSceneHook = rs.unwrap();
    s_replaceSceneHook->setAutoEnable(false);
    s_replaceSceneHook->disable();

    auto gws = self.getHook("cocos2d::CCDirector::getWinSize");
    if (!gws) {
      log::error("dum bass");
      return;
    }
    s_getWinSizeHook = gws.unwrap();
    s_getWinSizeHook->setAutoEnable(false);
    s_getWinSizeHook->disable();
  }
    */
  bool replaceScene(CCScene* scene) {
    if (s_replaceSceneHookEnabled) {
      auto transition = typeinfo_cast<CCTransitionScene*>(scene);
      if (transition) {
        s_requestedScene = static_cast<owo*>(transition)->m_fields->m_inScene;
      } else {
        s_requestedScene = scene;
      }
      return true;
    }
    return CCDirector::replaceScene(scene);
  }
  bool pushScene(CCScene* scene) {
    if (s_replaceSceneHookEnabled) {
      auto transition = typeinfo_cast<CCTransitionScene*>(scene);
      if (transition) {
        s_requestedScene = static_cast<owo*>(transition)->m_fields->m_inScene;
      } else {
        s_requestedScene = scene;
      }
      return true;
    }
    return CCDirector::pushScene(scene);
  }
  CCSize getWinSize() {
    auto ret = CCDirector::getWinSize();
    if (!s_getWinSizeHookEnabled) return ret;
    auto game = gdlazer::game::OsuGame::get(false);
    if (game && s_replaceSceneHookEnabled) {
      ret.height -= game->processUnit(ToolbarConstants::c_height, gdlazer::frameworks::Unit::UIKit, false);
    }
    return ret;
  }
};

GDL_NS_START
cocos2d::CCScene* captureIncomingScene(std::function<void()> dispatcher) {
  s_replaceSceneHookEnabled = true;
  s_getWinSizeHookEnabled = true;

  dispatcher();

  s_getWinSizeHookEnabled = false;
  s_replaceSceneHookEnabled = false;

  return s_requestedScene;
}
GDL_NS_END