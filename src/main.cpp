/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>

#include "Geode/cocos/keypad_dispatcher/CCKeypadDispatcher.h"
#include "game/overlays/dialog/PopupDialog.hpp"
#include "game/screens/menu/MainMenu.hpp"
#include "game/graphics/containers/WaveContainer.hpp"
//#include "helpers/sound/SoundManager.hpp"
//#include "game/graphics/containers/beatsync/BeatDetector.hpp"
#include "game/screens/menu/intro/IntroTriangles.hpp"
#include "utils.hpp"
#include "game/OsuGame.hpp"
#include "framework/input/events/KeyEvent.hpp"

/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
#define MAX_LEN         (cocos2d::kMaxLogLen + 1)
void CCLog2(char const * pszFormat, ...)
{
    char szBuf[MAX_LEN];

    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
    va_end(ap);

    WCHAR wszBuf[MAX_LEN] = {0};
    MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
    OutputDebugStringW(wszBuf);
    OutputDebugStringA("\n");

    WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
    log::info("[CCLog]: {}", szBuf);
}
#if 0
$execute {
    if (!Mod::get()->hook(
        reinterpret_cast<void*>(geode::base::getCocos()+0x73e10), // address
        &CCLog2, // detour
        "cocos2d::CCLog", // display name, shows up on the console
        tulip::hook::TulipConvention::Cdecl // calling convention
    ).isErr()) {
        log::debug("hook: Get ready for the brand new jam!");
    };
}
#endif
#endif

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

class AssetsLoadedEvent : public Event {
public:
    AssetsLoadedEvent() {};
};

#include <Geode/modify/LoadingLayer.hpp>
#include "game/graphics/ui/OsuText.hpp"
class $modify(Camila, LoadingLayer) {
    struct Fields {
        CCLabelTTF* m_smallLabel = nullptr;
        CCLabelTTF* m_smallLabel2 = nullptr;
        bool j = false;
    };
    bool init(bool idk) {
        bool res = LoadingLayer::init(idk);
        CCLog("meow");
        this->setAnchorPoint({0,2});
        this->ignoreAnchorPointForPosition(false);
        
        this->schedule(schedule_selector(Camila::updateSmallTextLabel));
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        m_fields->m_smallLabel = OsuText("",FontType::Regular, 14, CCTextAlignment::kCCTextAlignmentRight);
        m_fields->m_smallLabel->setPosition(winSize-CCPoint{30,-5-winSize.height});
        m_fields->m_smallLabel->setAnchorPoint({1,0});
        addChild(m_fields->m_smallLabel);
        m_fields->m_smallLabel2 = OsuText("",FontType::Regular, 10, CCTextAlignment::kCCTextAlignmentRight);
        m_fields->m_smallLabel2->setPosition(winSize-CCPoint{30,-22-winSize.height});
        m_fields->m_smallLabel2->setAnchorPoint({1,0});
        addChild(m_fields->m_smallLabel2);
        return res;
    }
    void updateProgress(int p0) {
        LoadingLayer::updateProgress(p0);
        if (p0 > 0) {
            if (!m_fields->j) AssetsLoadedEvent().post();
            m_fields->j = true;
        }
    }
    void updateSmallTextLabel(float the) {
        auto sl1 = static_cast<CCLabelBMFont*>(getChildByIDRecursive("geode-small-label"));
        // if the node somehow doesn't exists (geode runtime flag or user666 doing) 
        // then don't do anything
        if (!sl1) return;
        auto sl2 = static_cast<CCLabelBMFont*>(getChildByIDRecursive("geode-small-label-2"));
        m_fields->m_smallLabel->setString(sl1->getString());
        m_fields->m_smallLabel2->setString(sl2->getString());
    };

};

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }
        CCLog("e");
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
    }
    void onQuit(CCObject*s) {
        MenuLayer::onQuit(s);
        /*
        PopupDialog* b = PopupDialog::createSimpleDialog(
            "Are you sure you want to exit GD?",
            "Last chance to turn back",

            "lemme out i need to take a bath",
            "nvm the 20-20-20 rule sucks", [this](CCNode* s) {this->endGame(); }
        );
        b->show();
        */
    }
    void onMyButton2(CCObject*) {
        auto o = OsuGame::get();
        CCDirector::sharedDirector()->pushScene(o);
        auto j = IntroTriangles::create();
        if (j) o->pushScreen(j);
    //WaveContainer::create(OverlayColorScheme::Red,CCSprite::createWithSpriteFrameName("GJ_logo_001.png"))->show();
    }
};
/*
#include <Geode/modify/CCTouchDispatcher.hpp>
class $modify(CCTouchDispatcher) {
    bool ccTouchBegan(CCTouch * t, CCEvent * e) {
        log::debug("[hook: CCTouchDispatcher]: ccTouchBegan");
        return CCTouchDispatcher::ccTouchBegan(t, e);
    }
    void ccTouchEnded(CCTouch * t, CCEvent * e) {
        log::debug("[hook: CCTouchDispatcher]: ccTouchEnded");
        CCTouchDispatcher::ccTouchEnded(t, e);
    }
    void ccTouchesBegan(CCSet * t, CCEvent * e) {
        log::debug("[hook: CCTouchDispatcher]: ccTouchesBegan");
        CCTouchDispatcher::ccTouchesBegan(t, e);
    }
    void ccTouchesEnded(CCSet * t, CCEvent * e) {
        log::debug("[hook: CCTouchDispatcher]: ccTouchesEnded");
        CCTouchDispatcher::ccTouchesEnded(t, e);
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

#include "game/OsuGame.hpp"
#include "framework/input/events/KeyEvent.hpp"
#include "framework/input/events/MouseEvent.hpp"

#ifndef GEODE_IS_ANDROID
// fields does not work on non-CCNode :pensive:
bool m_click = false;
CCPoint lastKnownCursorPos = ccp(0,0);

#include <Geode/modify/CCEGLView.hpp>
class $modify(meowview, CCEGLView) {
    // it's all fucking black magic
    static void onModify(ModifyBase<ModifyDerive<meowview,CCEGLView>>& self) {
        if (auto mmc = self.getHook("cocos2d::CCEGLView::onGLFWMouseMoveCallBack")) {
            mmc.unwrap()->setAutoEnable(false);
        }
        if (auto mc = self.getHook("cocos2d::CCEGLView::onGLFWMouseCallBack")) {
            mc.unwrap()->setAutoEnable(false);
        }
        if (auto wsfc = self.getHook("cocos2d::CCEGLView::onGLFWWindowSizeFunCallback")) {
            wsfc.unwrap()->setAutoEnable(false);
        }
    }
    CCPoint getMousePosition() {return lastKnownCursorPos;}
    void onGLFWMouseMoveCallBack(GLFWwindow * window, double x, double y) {
        //log::debug("hi chat");
        CCEGLView::onGLFWMouseMoveCallBack(window, x, y);
        int w; int h;
        w = m_obScreenSize.width;
        h = m_obScreenSize.height;
        auto st = CCDirector::sharedDirector()->getVisibleSize();
        auto p = CCPoint(x / w * st.width, ((h-y) / h * st.height));
        lastKnownCursorPos = p;
        OsuGame::get()->dispatchEvent(new MouseEvent(MouseEventType::Move, CCPoint{ (float)p.x, (float)p.y }));
    };
    void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods) {
        CCEGLView::onGLFWMouseCallBack(window, button, action, mods);
        auto o = OsuGame::get();
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            m_click = true;
            o->dispatchEvent(new MouseEvent(MouseEventType::MouseDown, lastKnownCursorPos));
            return;
        }
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && m_click) {
            o->dispatchEvent(new MouseEvent(MouseEventType::MouseUp, lastKnownCursorPos));
            m_click = false;
        }
    };
    void onGLFWWindowSizeFunCallback(GLFWwindow* window, int width, int height) {
        CCEGLView::onGLFWWindowSizeFunCallback(window, width, height);
        OsuGame::get()->setContentSize(getDesignResolutionSize());
    }
};

#include <Geode/modify/CCMouseDispatcher.hpp>
class $modify(neko, CCMouseDispatcher) {
    static void onModify(ModifyBase<ModifyDerive<neko,CCMouseDispatcher>>& self) {
        if (auto e = self.getHook("cocos2d::CCMouseDispatcher::dispatchScrollMSG")) {
            e.unwrap()->setAutoEnable(false);
        }
    }
    bool dispatchScrollMSG(float x, float y) {
        // what the fuck why is vertical gets passed in as a horizontal value
        OsuGame::get()->dispatchEvent(new MouseEvent(MouseEventType::MouseScroll, {y,x}));
        return CCMouseDispatcher::dispatchScrollMSG(x,y);
    }
};

#include <Geode/modify/CCKeyboardDispatcher.hpp>
class $modify(catdispatch, CCKeyboardDispatcher) {
    static void onModify(ModifyBase<ModifyDerive<catdispatch,CCKeyboardDispatcher>>& self) {
        if (auto e = self.getHook("cocos2d::CCKeyboardDispatcher::dispatchKeyboardMSG")) {
            e.unwrap()->setAutoEnable(false);
        }
    }
    bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) {
        auto ret = CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);
        OsuGame::get()->dispatchEvent(new KeyboardEvent(KeyInfo{
            .key = key,
            .ctrl = getControlKeyPressed() || getCommandKeyPressed(),
            .alt = getAltKeyPressed(),
            .shift = getShiftKeyPressed(),
            .pressed = isKeyDown
        }));
        return ret;
    }
};

#else
#include <Geode/modify/CCTouchDispatcher.hpp>
class $modify(the,CCTouchDispatcher) {
    static void onModify(ModifyBase<ModifyDerive<the,CCTouchDispatcher>>& self) {
        if (auto e = self.getHook("cocos2d::CCTouchDispatcher::touches")) {
            e.unwrap()->setAutoEnable(false);
        }
    }
    void broadcastPos(MouseEventType type, CCPoint pos) {
        OsuGame::get()->dispatchEvent(new MouseEvent(type,pos));
    };
    void touches(CCSet * t, CCEvent * e, unsigned int idx) {
        CCTouchDispatcher::touches(t, e, idx);
        MouseEventType type;
        switch(idx) {
            case CCTOUCHBEGAN: type = MouseEventType::MouseDown; break;
            case CCTOUCHMOVED: type = MouseEventType::Move; break;
            case CCTOUCHENDED: 
            case CCTOUCHCANCELLED: type = MouseEventType::MouseUp; break;
        }
        the::broadcastPos(type,static_cast<CCTouch*>(*t->begin())->getLocation());
    }
};

#include <Geode/modify/CCKeypadDispatcher.hpp>
class $modify(mimimi, CCKeypadDispatcher) {
    void dispatchKeypadMSG(ccKeypadMSGType nMsgType) {
        CCKeypadDispatcher::dispatchKeypadMSG(nMsgType);
        OsuGame::get()->dispatchEvent(new KeypadEvent(key));
    }
};
#endif 

$execute {
    new EventListener<EventFilter<AssetsLoadedEvent>>(+[](AssetsLoadedEvent* e){
        for (auto* hook : Mod::get()->getHooks()) {
            if (hook->getDisplayName().starts_with("cocos2d::CCEGLView")) hook->enable();
            if (hook->getDisplayName().starts_with("cocos2d::CCKeyboardDispatcher")) hook->enable();
            if (hook->getDisplayName().starts_with("cocos2d::CCMouseDispatcher")) hook->enable();
            if (hook->getDisplayName().starts_with("cocos2d::CCTouchDispatcher")) hook->enable();
        }
        return ListenerResult::Propagate;
    });
}
