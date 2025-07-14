#include "game/OsuGame.hpp"
#include "game/screens/menu/intro/IntroTriangles.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>

bool g_screenPushed = false;
struct e : public Modify<e, MenuLayer> {
  bool init() {
    if (!MenuLayer::init()) return false;

    auto myButton = CCMenuItemSpriteExtra::create(
      CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
      this,
      menu_selector(e::onMyButton)
    );
    auto menu = this->getChildByID("bottom-menu");
    menu->addChild(myButton);

    myButton->setID("my-button"_spr);

    menu->updateLayout();

    auto d = CCDirector::get();

    

    return true;
  };
  void onMyButton(CCObject*) {
    auto g = gdlazer::game::OsuGame::get();
    cocos2d::CCDirector::get()->pushScene(g);
    if (!g_screenPushed) g->pushScreen(GDL_NS::IntroTriangles::create());
    g_screenPushed = true;
  }
};

void showCursor(bool state) {
#ifdef GEODE_IS_DESKTOP
  cocos2d::CCDirector::get()->getOpenGLView()->showCursor(false);
#endif
}

bool s_replaceSceneDisabled = false;


geode::Hook* g_showCursorHook = nullptr;
#ifdef GEODE_IS_DESKTOP
#include <Geode/modify/CCEGLView.hpp>
struct showCursorHook : public geode::Modify<showCursorHook, CCEGLView> {
  static void onModify(auto& self) {
    auto hook = self.getHook("cocos2d::CCEGLView::showCursor");
    
    g_showCursorHook = hook.unwrapOr(nullptr);
    if (g_showCursorHook) {
      g_showCursorHook->setAutoEnable(false);
    } else {
      log::error("showCursor hook is dead");
    }
  }
  void showCursor(bool state) {
    if (auto instance = gdlazer::game::OsuGame::get(false)) {
      instance->setMouseVisibility(state);
    }
  }
};
#endif

#include <Geode/modify/CCDirector.hpp>
class $modify(cocos2d::CCDirector) {
  static void onModify(auto& self) {
    if (!self.setHookPriorityPre("cocos2d::CCDirector::replaceScene", Priority::VeryEarlyPre)) {
      geode::log::warn("stuypd"); 
    };
  }
  bool pushScene(cocos2d::CCScene* scene) {
    bool isgdlScene = scene == gdlazer::game::OsuGame::get(false);
    if (g_showCursorHook) {
      if (isgdlScene) getOpenGLView()->showCursor(false);
      if ((
        (isgdlScene) 
        ? g_showCursorHook->enable() 
        : g_showCursorHook->disable()
      ).isErr()) {
        geode::log::error("Cannot change state of cocos2d::CCEGLView::showCursor hook.");
      } else {
        if (!isgdlScene) getOpenGLView()->showCursor(true);
      }
    }
    return cocos2d::CCDirector::pushScene(scene);
  }
  bool replaceScene(cocos2d::CCScene* scene) {
    if (!s_replaceSceneDisabled) {
      bool isgdlScene = scene == gdlazer::game::OsuGame::get(false);
      if (g_showCursorHook) {
        if (isgdlScene) getOpenGLView()->showCursor(false);
        if ((
          (isgdlScene) 
          ? g_showCursorHook->enable() 
          : g_showCursorHook->disable()
        ).isErr()) {
          geode::log::error("Cannot change state of cocos2d::CCEGLView::showCursor hook.");
        } else {
          if (!isgdlScene) getOpenGLView()->showCursor(true);
        }
      }
      return cocos2d::CCDirector::replaceScene(scene);
    }
    return true;
  }
};

#include <Geode/modify/GameManager.hpp>
class $modify(GameManager) {
  void fadeInMenuMusic() {
    if (!s_replaceSceneDisabled) {
    }
  }
};
#include <Geode/modify/LevelBrowserLayer.hpp>
class $modify(LevelBrowserLayer) {
  void onBack(cocos2d::CCObject*) {
    cocos2d::CCDirector::get()->replaceScene(gdlazer::game::OsuGame::get());
  }
};

#include <Geode/modify/LoadingLayer.hpp>
class $modify(LoadingLayer) {
  static void onModify(auto& self) {
    if (!self.setHookPriorityPre("LoadingLayer::loadAssets", Priority::VeryLatePre)) {
      geode::log::warn("Failed to set hook priority for LoadingLayer::loadAssets.");
    }
    if (!self.setHookPriorityAfterPost("LoadingLayer::init", Loader::get()->getLoadedMod("geode.loader"))) {
      geode::log::warn("Failed to set hook priority for LoadingLayer::init.");
    }
  }
  void loadAssets() {
    bool lastStep = m_loadStep == 0xe;
    if (lastStep) {
      s_replaceSceneDisabled = true;
    }
    LoadingLayer::loadAssets();
    if (lastStep) {
      s_replaceSceneDisabled = false;

      auto g = gdlazer::game::OsuGame::get();
      cocos2d::CCDirector::get()->pushScene(g);
      if (!g_screenPushed) g->pushScreen(GDL_NS::IntroTriangles::create());
      g_screenPushed = true;
      if (g_showCursorHook && g_showCursorHook->enable().isErr()) {
        geode::log::error("Cannot enable cocos2d::CCEGLView::showCursor hook");
      };
    }
  }
  bool init(bool p0) {
    if (!LoadingLayer::init(p0)) return false;
    cocos2d::CCDirector::get()->getOpenGLView()->showCursor(false);

    auto yoffset = CCDirector::get()->getWinSize().height * 2;
    auto xpos = CCDirector::get()->getWinSize().width - 10;

    auto smallLabel = static_cast<cocos2d::CCLabelBMFont*>(getChildByID("geode-small-label"));
    auto smallerLabel = static_cast<cocos2d::CCLabelBMFont*>(getChildByID("geode-small-label-2"));

    setVisible(false);

    queueInMainThread([this, smallLabel, smallerLabel, yoffset, xpos]{
      smallLabel->setPosition({xpos, 10 + yoffset});
      smallLabel->setAnchorPoint({1,0});
      smallLabel->setFntFile("Torus-Regular.fnt"_spr);
      smallLabel->setScale(.3f);
      
      smallerLabel->setPosition({xpos, 25 + yoffset});
      smallerLabel->setFntFile("Torus-Regular.fnt"_spr);
      smallerLabel->setAnchorPoint({1,0});
      smallerLabel->setScale(.2f);
      
      setPositionY(-yoffset);
      setVisible(true);
    });
    return true;
  }
};


bool g_devToolsShowCursor = false;
#include <Geode/modify/CCKeyboardDispatcher.hpp>
class $modify(CCKeyboardDispatcher) {
  bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool arr) {
    if (down && (key == KEY_F11 GEODE_MACOS(|| key == KEY_F10))) {
      g_showCursorHook->disable();
      CCEGLView::get()->showCursor(g_devToolsShowCursor = !g_devToolsShowCursor);
      g_showCursorHook->enable();
    }
    return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, arr);
  }
};