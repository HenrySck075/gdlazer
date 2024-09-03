/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include "game/screens/menu/intro/IntroTriangles.hpp"
#include "game/overlays/dialog/PopupDialog.hpp"
#include "game/screens/menu/MainMenu.hpp"
#include "game/graphics/containers/WaveContainer.hpp"
//#include "helpers/sound/SoundManager.hpp"
//#include "game/graphics/containers/beatsync/BeatDetector.hpp"
#include "utils.hpp"
#include "game/OsuGame.hpp"
#include "framework/input/events/KeyEvent.hpp"

#include "game/input/Input.hpp"
/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;


#include <Geode/modify/PauseLayer.hpp>
class $modify(nPauseLayer,PauseLayer) {
    struct Fields {
        FMOD::Channel* c;
        FMOD::Sound* s;
    };
    void scheduleBGM() {
        auto engine = FMODAudioEngine::sharedEngine();
        engine->m_backgroundMusicChannel->setPaused(false);
        //engine->pauseMusic(1);
        auto sys = engine->m_system;
        sys->createSound("pause-loop.mp3"_spr, FMOD_LOOP_NORMAL, nullptr, &(m_fields->s));
        engine->m_system->playSound(m_fields->s, nullptr, false, &(m_fields->c));
        m_fields->c->setVolume(1);
    }
    void onResume(CCObject* c) {
        auto engine = FMODAudioEngine::sharedEngine();
        engine->pauseMusic(4761);
        PauseLayer::onResume(c);
    }
    void customSetup() {
        PauseLayer::customSetup();

    /*
    CCObject* obj;
    CCARRAY_FOREACH(this->getChildren(), obj) {
        auto n = static_cast<CCNode*>(obj);
        //int oldOpacity = n->getOpacity();
        n->setVisible(false);
        //n->runAction(CCFadeTo::create(0.2, oldOpacity));
    }
    */
        int opacity = this->getOpacity();
        this->setOpacity(0);
        this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.05), CCCallFunc::create(this, callfunc_selector(nPauseLayer::scheduleBGM))));
        this->runAction(CCFadeTo::create(0.25,opacity));
    }
};

/**
 * `$modify` lets you extend and modify GD's classes.
 * To hook a function in Geode, simply $modify the class
 * and write a new function definition with the signature of
 * the function you want to hook.
 *
 * Here we use the overloaded `$modify` macro to set our own class name,
 * so that we can use it for button callbacks.
 *
 * Notice the header being included, you *must* include the header for
 * the class you are modifying, or you will get a compile error.
 *
 * Another way you could do this is like this:
 *
 * struct MyMenuLayer : Modify<MyMenuLayer, MenuLayer> {};
 */

#include <Geode/modify/LoadingLayer.hpp>
class $modify(LoadingLayer) {
    bool init(bool idk) {
        bool res = LoadingLayer::init(idk);
        this->setVisible(false);
        return res;
    }
};

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
    /**
     * Typically classes in GD are initialized using the `init` function, (though not always!),
     * so here we use it to add our own button to the bottom menu.
     *
     * Note that for all hooks, your signature has to *match exactly*,
     * `void init()` would not place a hook!
    */
    bool init() {
        /**
         * We call the original init function so that the
         * original class is properly initialized.
         */
        if (!MenuLayer::init()) {
            return false;
        }

        auto introButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
            this,
            /**
             * Here we use the name we set earlier for our modify class.
            */
            menu_selector(MyMenuLayer::onMyButton)
        );


        auto dialogButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
            this,
            /**
             * Here we use the name we set earlier for our modify class.
            */
            menu_selector(MyMenuLayer::onMyButton2)
        );

        auto menu = this->getChildByID("bottom-menu");
        menu->addChild(introButton);
        menu->addChild(dialogButton);

        /**
         * We update the layout of the menu to ensure that our button is properly placed.
         * This is yet another Geode feature, see this page for more info about it:
         * https://docs.geode-sdk.org/tutorials/layouts
        */
        menu->updateLayout();

        //glfwSetWindowFocusCallback();
        /**
         * We return `true` to indicate that the class was properly initialized.
         */
        return true;
    }

    /**
     * This is the callback function for the button we created earlier.
     * The signature for button callbacks must always be the same,
     * return type `void` and taking a `CCObject*`.
    */
    void onMyButton(CCObject*) {
        CCDirector::get()->pushScene(osuIntroTriangles::create());
    }
    void onQuit(CCObject*) {
        PopupDialog* b = PopupDialog::createSimpleDialog(
            "Are you sure you want to exit GD?",
            "Last chance to turn back",

            "lemme out i need to take a bath",
            "nvm the 20-20-20 rule sucks", [this](CCNode* s) {this->endGame(); }
        );
        b->show();
    }
    void onMyButton2(CCObject*) {
        auto o = OsuGame::get();
        CCDirector::sharedDirector()->pushScene(o);
        o->pushScreen(MainMenu::create(false));
    //WaveContainer::create(OverlayColorScheme::Red,CCSprite::createWithSpriteFrameName("GJ_logo_001.png"))->show();
    }
};
/*
#include <Geode/modify/CCTouchDelegate.hpp>
class $modify(CCTouchDelegate) {
    bool ccTouchBegan(CCTouch * t, CCEvent * e) {
        log::debug("[hook: CCTouchDelegate]: ccTouchBegan");
        return CCTouchDelegate::ccTouchBegan(t, e);
    }
    void ccTouchEnded(CCTouch * t, CCEvent * e) {
        log::debug("[hook: CCTouchDelegate]: ccTouchEnded");
        CCTouchDelegate::ccTouchEnded(t, e);
    }
    void ccTouchesBegan(CCSet * t, CCEvent * e) {
        log::debug("[hook: CCTouchDelegate]: ccTouchesBegan");
        CCTouchDelegate::ccTouchesBegan(t, e);
    }
    void ccTouchesEnded(CCSet * t, CCEvent * e) {
        log::debug("[hook: CCTouchDelegate]: ccTouchesEnded");
        CCTouchDelegate::ccTouchesEnded(t, e);
    }
};
class BeatUpdater : public CCNode {
private:
  BeatDetector* instance;
  TimeStamp* lastBeat;
  void m() {
    if (instance->isPlaying()) {
      instance->update();
    }
  }

public:
  static BeatUpdater* create() {
    create_class(BeatUpdater, init);
  };
  bool init() {
    instance = BeatDetector::Instance();
    this->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(
      CCDelayTime::create(0.1),
      CCCallFunc::create(this, callfunc_selector(BeatUpdater::m)))
    ));
    return true;
  }
};

$execute{
    SceneManager::get()->keepAcrossScenes(BeatUpdater::create());

}

#include <Geode/modify/FMODAudioEngine.hpp>
class $modify(FMODAudioEngine) {
    void playMusic(std::string path, bool shouldLoop, float fadeInTime, int channel) {
        FMODAudioEngine::playMusic(path, shouldLoop, fadeInTime, channel);
        auto instance = BeatDetector::Instance();
        if (!instance->systemLoaded()) instance->loadSystem();
        instance->LoadSongFromSystem();
    }
};
*/


#include <Geode/modify/CCNodeRGBA.hpp>
class $modify(CCNodeRGBA) {
    virtual void updateDisplayedOpacity(GLubyte parentOpacity) {
        _displayedOpacity = _realOpacity * parentOpacity/255.0;
        
        if (_cascadeOpacityEnabled)
        {
            CCObject* pObj;
            CCARRAY_FOREACH(m_pChildren, pObj)
            {
                if (CCRGBAProtocol* item = typeinfo_cast<CCRGBAProtocol*>(pObj))
                {
                    CCBool* _b = static_cast<CCBool*>(typeinfo_cast<CCNode*>(item)->getUserObject("opacityCascadeBlacklist"));
                    
                    if (!(_b!=nullptr && _b->getValue())) item->updateDisplayedOpacity(_displayedOpacity);
                }
            }
        }
    }
};

#include <Geode/modify/CCLayerRGBA.hpp>
class $modify(CCLayerRGBA) {
    virtual void updateDisplayedOpacity(GLubyte parentOpacity) {
        _displayedOpacity = _realOpacity * parentOpacity/255.0;
        
        if (_cascadeOpacityEnabled)
        {
            CCObject* pObj;
            CCARRAY_FOREACH(m_pChildren, pObj)
            {
                if (CCRGBAProtocol* item = typeinfo_cast<CCRGBAProtocol*>(pObj))
                {
                    CCBool* _b = static_cast<CCBool*>(typeinfo_cast<CCNode*>(item)->getUserObject("opacityCascadeBlacklist"));
                    
                    if (!(_b!=nullptr && _b->getValue())) item->updateDisplayedOpacity(_displayedOpacity);
                }
            }
        }
    }
};
