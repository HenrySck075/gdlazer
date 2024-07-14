/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include "intro/IntroTriangles.hpp"
#include "ui/Dialog.hpp"
#include "helpers/MouseEvent.hpp"
#include "main/PauseLayer.hpp"

/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;

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

		auto d = static_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("close-button"));

    if (d!=nullptr) {
      auto replaceBtn = CCMenuItemSpriteExtra::create(d->getNormalImage(), this, menu_selector(MyMenuLayer::onMyButton2));
      replaceBtn->setPosition(d->getPosition());
      this->getChildByID("close-menu")->addChild(replaceBtn);
      d->removeFromParent();
    }

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
	void onMyButton2(CCObject*) {
		osuDialog::create(
			"Are you sure you want to exit GD?", 
			"Last chance to turn back", 
			{ 
				DialogButton::create("my mom called me for dinner", dialog_button_primary, "dialog-ok-select.wav"_spr),
				DialogButton::create("clicked the wrong button mb", dialog_button_secondary, "dialog-cancel-select.wav"_spr) 
			}
		)->show();
	}
};

#ifndef GEODE_IS_ANDROID

#include <Geode/modify/CCEGLView.hpp>
class $modify(CCEGLView) {
	void onGLFWMouseMoveCallBack(GLFWwindow * window, double x, double y) {
		CCEGLView::onGLFWMouseMoveCallBack(window, x, y);
		int w; int h;
		w = m_obScreenSize.width;
		h = m_obScreenSize.height;
		auto st = CCDirector::sharedDirector()->getVisibleSize();
		auto p = CCPoint(x / w * st.width, ((h-y) / h * st.height));
		MouseEvent(CCPoint{ (float)p.x, (float)p.y }).post();
	};
	void onGLFWMouseCallback(GLFWwindow* window, int button, int action, int mods) {
		CCEGLView::onGLFWMouseCallBack(window, button, action, mods);
	}
};
#endif // DEBUG
