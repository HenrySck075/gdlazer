#include "Game.hpp"
#include "graphics/containers/FillFlowContainer.hpp"

GDL_NS_START
static geode::Ref<Game> s_instance;

bool Game::dispatchEvent(Event *event) {
  // Handle the event
  if (!EventTarget::dispatchEvent(event)) {
    return false;
  }

  // Propagate to children
  if (!event->canPropagate())
    return true;
  auto children = getChildren();
  if (children) {
    for (auto child : geode::cocos::CCArrayExt<CCNode *>(children)) {
      auto eventTarget = dynamic_cast<EventTarget *>(child);
      if (eventTarget && !eventTarget->dispatchEvent(event)) {
        return false;
      }
    }
  }
  return true;
}
bool Game::init() {
    if (!CCScene::init()) return false;
    auto flowContainer = FillFlowContainer::create(FillDirection::Horizontal);
    for (int i = 0; i < 5; i++) {
        auto us = Container::create();
        us->setSize({120,50});
        us->setBorderRadius(10);
        us->setBackgroundColor({255,255,255,255});
        flowContainer->addChild(us);
    }
    this->addChild(flowContainer);
    flowContainer->updateLayout();

    auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(Game::yeah)
		);
		auto menu = cocos2d::CCMenu::create();
		menu->addChild(myButton);

		myButton->setID("my-button"_spr);

    this->addChild(menu);

    return true;
};
geode::Ref<Game> Game::get() {
  if (!s_instance) {
      s_instance = new Game();
      s_instance->init();
  }
  return s_instance;
};
GDL_NS_END 