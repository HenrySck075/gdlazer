#include "Game.hpp"
#include "graphics/containers/FillFlowContainer.hpp"
#include "graphics/containers/ScrollableContainer.hpp"
#include <mutex>

GDL_NS_START
static geode::Ref<Game> s_instance;

bool Game::init() {
    if (!CCScene::init()) return false;

    /*TEST CODE ~ Remove after finished*/
    auto flowContainer = FillFlowContainer::create(FillDirection::Horizontal);
    for (int i = 0; i < 5; i++) {
      auto us = Container::create();
      us->setSize({120,50});
      us->setBorderRadius(10);
      us->setBackgroundColor({255,255,255,255});
      flowContainer->addChild(us);
      us->addListener<MouseEvent>([us](MouseEvent* e){
        us->runAction(cocos2d::CCSequence::createWithTwoActions(
          cocos2d::CCTintTo::create(0, 255, 0, 0),
          cocos2d::CCTintTo::create(0.5, 255, 255, 255)
        ));
        return true;
      });
    }
    
    //auto scroll = ScrollableContainer::create();
    this->addChild(flowContainer);
    //this->addChild(scroll);
    flowContainer->updateLayout();
    //scroll->setSize({300, flowContainer->getContentSize().height});
    
		auto menu = cocos2d::CCMenu::create();
    auto returnButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(Game::yeah)
		);
		menu->addChild(returnButton);

    auto devtoolsButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(Game::weh)
		);
		menu->addChild(devtoolsButton);
    devtoolsButton->setPosition({200,0});

    this->addChild(menu);

    return true;
};

std::mutex g_fish;
Game* Game::get(bool create) {
  if (!s_instance && create) {
    g_fish.lock();
    s_instance = new Game();
    s_instance->autorelease();
    s_instance->init();
    g_fish.unlock();
  }
  return s_instance;
};
GDL_NS_END bool gdlazer::Game::doDispatchEvent(Event *event,
                                               std::type_index type) {
  geode::log::debug("Dispatching {} to {}'s handlers", getObjectName(event),
                    getObjectName(this));
  geode::Ref<Event> eventRefHolder(event);
  // Handle the event
  if (!EventTarget::doDispatchEvent(event, type)) {
    return false;
  }

  // Propagate to children
  if (event->m_propagateStopped) {
    return true;
  }
  auto children = getChildren();
  if (children) {
    for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode>(children)) {
      geode::log::debug("1");
      auto eventTarget = geode::cast::typeinfo_cast<Container *>(child);
      geode::log::debug("2");
      if (eventTarget != nullptr)
        continue;
      if (!eventTarget->dispatchEvent(event)) {
        return false;
      }
    }
  }
  // idk i think this would keep the object around
  auto idk = eventRefHolder.data();
  return true;
};
