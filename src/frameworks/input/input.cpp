#include <Geode/Geode.hpp>
#include <cmath>

#include "events/MouseEvent.hpp"
#include "../Game.hpp"

bool mouseClicked = false;
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
		auto st = CCDirector::sharedDirector()->getVisibleSize();
		auto p = CCPoint(x / w * st.width, ((h-y) / h * st.height));
		g_mousePos = p;
		auto g = gdlazer::Game::get(false);
    if (g || cocos2d::CCScene::get() == g) g->dispatchEvent(new gdlazer::MouseEvent(
			gdlazer::MouseEventType::Move, p, mouseClicked
		));
		CCEGLView::onGLFWMouseMoveCallBack(window, x, y);
  };
	void onGLFWMouseCallBack(GLFWwindow*, int button, int action, int mods) {
		auto g = gdlazer::Game::get(false);
    if (g || cocos2d::CCScene::get() == g) g->dispatchEvent(new gdlazer::MouseEvent(
			action == GLFW_PRESS ? gdlazer::MouseEventType::MouseDown : gdlazer::MouseEventType::MouseUp,
			g_mousePos, true
		));
		mouseClicked = action == GLFW_PRESS;
		CCEGLView::onGLFWMouseCallBack(window, button, action, mods);
	}
};
#else
#include <Geode/modify/CCTouchDispatcher.hpp>
float g_moveThreshold = 0.5;
struct m : public geode::Modify<m, cocos2d::CCTouchDispatcher> {
  void touches(cocos2d::CCSet* touches, cocos2d::CCEvent* e, unsigned int index) {
    auto touch = static_cast<cocos2d::CCTouch*>(*touches->begin());
    if (
      (!touch->m_startPointCaptured || index == CCTOUCHENDED) ||
      fabsf(touch->m_startPoint.getDistance(touch->m_point)) >= g_moveThreshold
    ) {
      if (index == CCTOUCHBEGAN) mouseClicked = true;
      if (index == CCTOUCHENDED) mouseClicked = false;
      auto g = gdlazer::Game::get(false);
      if (g!=nullptr && cocos2d::CCDirector::get()->getRunningScene() == g) g->dispatchEvent(new gdlazer::MouseEvent(
        index == CCTOUCHBEGAN ? gdlazer::MouseEventType::MouseDown : 
        index == CCTOUCHMOVED ? gdlazer::MouseEventType::MouseDown : gdlazer::MouseEventType::MouseUp,
        touch->m_point, mouseClicked
      ));
    }

    CCTouchDispatcher::touches(touches, e, index);
  }
};
#endif
