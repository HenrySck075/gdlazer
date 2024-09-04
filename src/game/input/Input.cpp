#include "Input.hpp"
#include "../OsuGame.hpp"
#include "../../framework/input/events/KeyEvent.hpp"
#include "../../framework/input/events/MouseEvent.hpp"
using namespace geode::prelude;

#ifndef GEODE_IS_ANDROID

// fields does not work on non-CCNode :pensive:
bool m_click = false;
CCPoint lastKnownCursorPos = ccp(0,0);

cocos2d::CCPoint& getLastKnownCursorPos() {
    return lastKnownCursorPos;
};
#include <Geode/modify/CCEGLView.hpp>
class $modify(meowview, CCEGLView) {
    // it's all fucking black magic
    /*
    static void onModify(ModifyBase<ModifyDerive<meowview,CCEGLView>>& self) {
        if (auto mmc = self.getHook("CCEGLView::onGLFWMouseMoveCallback")) {
            mmc.unwrap()->setAutoEnable(false);
        }
        if (auto mc = self.getHook("CCEGLView::onGLFWMouseCallBack")) {
            mc.unwrap()->setAutoEnable(false);
        }
    }
    */
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
        OsuGame::get()->dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
    }
};


#include <Geode/modify/CCKeyboardDispatcher.hpp>
class $modify(catdispatch, CCKeyboardDispatcher) {
    static void onModify(ModifyBase<ModifyDerive<catdispatch,CCKeyboardDispatcher>>& self) {
        if (auto e = self.getHook("CCKeyboardDispatcher::dispatchKeyboardMSG")) {
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

#elif false
#include <Geode/modify/CCTouchDispatcher.hpp>
class $modify(CCTouchDispatcher) {
    void broadcastPos(CCPoint pos) {
        OsuGame::get()->dispatchEvent(new MouseEvent(MouseEventType::Move,pos));
    };
    void touches(CCSet* t, CCEvent* e, uint i) {
        CCTouchDispatcher::touches(t, e, i);
        if (t->count()!=0) broadcastPos(static_cast<CCTouch*>(t->anyObject())->getLocation());
    }
};

#endif 

bool hookEnabled = false;

void hookInput() {
    /*
    if (hookEnabled) return;
    hookEnabled = true;

    for (auto* i : Mod::get()->getHooks()) {
        auto d = i->getDisplayName();
        if (d.starts_with("CCEGLView")) i->enable();
        else if (d.starts_with("CCKeyboardDispatcher")) i->enable();
    }
    log::info("[hookInput]: Manually hooked input stuff. idk why but yeah ig it works now");
    */
};



