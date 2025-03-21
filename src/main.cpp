#include "frameworks/input/events/MouseEvent.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include "frameworks/Game.hpp"
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

        return true;
    };
    void onMyButton(CCObject*) {
		cocos2d::CCDirector::get()->pushScene(gdlazer::Game::get());
	}
};


cocos2d::CCPoint g_mousePos;
bool mouseClicked = false;
#include <Geode/modify/CCEGLView.hpp>
struct m : public Modify<m, cocos2d::CCEGLView> {
	/*
	void onGLFWKeyCallback(
		GLFWwindow*, int key, int scancode, int action, int mods
	) {}
	*/
	void onGLFWMouseMoveCallBack(GLFWwindow * window, double x, double y) {
		//log::debug("hi chat");
		CCEGLView::onGLFWMouseMoveCallBack(window, x, y);
		int w; int h;
		w = m_obScreenSize.width;
		h = m_obScreenSize.height;
		auto st = CCDirector::sharedDirector()->getVisibleSize();
		auto p = CCPoint(x / w * st.width, ((h-y) / h * st.height));
		g_mousePos = p;
		gdlazer::Game::get()->dispatchEvent(new gdlazer::MouseEvent(
			gdlazer::MouseEventType::Move, p, mouseClicked
		));
	  };
	void onGLFWMouseCallBack(GLFWwindow*, int button, int action, int mods) {
		gdlazer::Game::get()->dispatchEvent(new gdlazer::MouseEvent(
			action == GLFW_PRESS ? gdlazer::MouseEventType::MouseDown : gdlazer::MouseEventType::MouseUp,
			g_mousePos, true
		));
		mouseClicked = true;
	}
};