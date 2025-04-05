#include <Geode/Geode.hpp>

#include "events/KeyEvent.hpp"
#include "events/MouseEvent.hpp"
#include "../Game.hpp"

using namespace gdlazer::framework;
using namespace geode;

// ============ Mouse input ============

bool g_mouseClicked = false;
#ifdef GEODE_IS_WINDOWS
cocos2d::CCPoint g_mousePos;
#include <Geode/modify/CCEGLView.hpp>
struct m : public Modify<m, cocos2d::CCEGLView> {
	/*
	void onGLFWKeyCallback(
		GLFWwindow*, int key, int scancode, int action, int mods
	) {}
	*/
	void onGLFWMouseMoveCallBack(GLFWwindow * window, double x, double y) {
		//log::debug("hi chat");
		int w; int h;
		w = m_obScreenSize.width;
		h = m_obScreenSize.height;
		auto st = CCDirector::get()->getVisibleSize();
		auto p = CCPoint(x / w * st.width, ((h-y) / h * st.height));
		g_mousePos = p;
		auto g = Game::get(false);
    if (g!=nullptr && cocos2d::CCScene::get() == g) g->dispatchEvent(new MouseEvent(
			MouseEventType::Move, p, g_mouseClicked
		));
		CCEGLView::onGLFWMouseMoveCallBack(window, x, y);
  };
	void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods) {
		auto g = Game::get(false);
    g_mouseClicked = action == GLFW_PRESS;
    if (g!=nullptr && cocos2d::CCScene::get() == g) g->dispatchEvent(new MouseEvent(
			action == GLFW_PRESS ? MouseEventType::MouseDown : MouseEventType::MouseUp,
			g_mousePos, g_mouseClicked
		));
		CCEGLView::onGLFWMouseCallBack(window, button, action, mods);
	}
};

#include <Geode/modify/CCMouseDispatcher.hpp>
struct md : public geode::Modify<md, CCMouseDispatcher> {
  bool dispatchScrollMSG(float x, float y) {
    auto g = Game::get(false);
    if (g!=nullptr && cocos2d::CCScene::get() == g) g->dispatchEvent(new MouseScrollEvent({x,y}, g_mousePos, g_mouseClicked));
    return CCMouseDispatcher::dispatchScrollMSG(x, y);
  }
};
#else
#include <Geode/modify/CCTouchDispatcher.hpp>
float g_moveThreshold = 0.5;
struct m : public geode::Modify<m, cocos2d::CCTouchDispatcher> {
  cocos2d::CCPoint convertPoint(cocos2d::CCPoint point) {
    return {point.x, CCDirector::get()->getWinSize().height - point.y};
    //auto obScreenSize = cocos2d::CCDirector::get()->getOpenGLView()->getFrameSize();
    //int w = obScreenSize.width; int h = obScreenSize.height;
    auto scalew = CCDirector::get()->getOpenGLView()->getScaleX();
    auto scaleh = CCDirector::get()->getOpenGLView()->getScaleY();
    //auto st = CCDirector::get()->getWinSize();
    return CCPoint(
      (point.x /*/ w * st.width*/) / scalew, 
      (CCDirector::get()->getWinSize().height - point.y/*/ h * st.height*/) / scaleh
    ); 
  }
  void touches(cocos2d::CCSet* touches, cocos2d::CCEvent* e, unsigned int index) {
    auto touch = static_cast<cocos2d::CCTouch*>(*touches->begin());
    auto pos = convertPoint(touch->m_point);
    /*if (
      (!touch->m_startPointCaptured || index == CCTOUCHENDED) ||
      fabsf(
        convertPoint(touch->m_startPoint)
        .getDistance(pos)
      )>= g_moveThreshold
    ) */{
      if (index == CCTOUCHBEGAN) g_mouseClicked = true;
      if (index == CCTOUCHENDED) g_mouseClicked = false;
      auto g = Game::get(false);
      if (g!=nullptr && cocos2d::CCScene::get() == g) g->dispatchEvent(new MouseEvent(
        index == CCTOUCHBEGAN ? MouseEventType::MouseDown : 
        index == CCTOUCHMOVED ? MouseEventType::Move : MouseEventType::MouseUp,
        pos, g_mouseClicked
      ));
    }

    CCTouchDispatcher::touches(touches, e, index);
  }
};
#endif
// ============ Keyboard input ============

#include <Geode/modify/CCKeyboardDispatcher.hpp>
struct kd : public geode::Modify<kd, CCKeyboardDispatcher> {
  bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool down, bool repeat) {
    auto g = Game::get(false);
    if (g!=nullptr && cocos2d::CCScene::get() == g) g->dispatchEvent(new KeyEvent(key, down));
    return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
  }
};
