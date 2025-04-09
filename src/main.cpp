#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include "frameworks/Game.hpp"
#include "game/screens/PlaygroundScreen.hpp"
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

    log::debug("{} {}", d->getWinSize(), cocos2d::CCDirector::get()->getOpenGLView()->getFrameSize());

    return true;
  };
  void onMyButton(CCObject*) {
    auto g = gdlazer::frameworks::Game::get();
    cocos2d::CCDirector::get()->pushScene(g);
    g->pushScreen(GDL_NS::PlaygroundScreen::create());
  }
};

