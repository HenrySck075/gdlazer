#include "Game.hpp"
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"
#include "Geode/loader/Loader.hpp"
#include "graphics/containers/FillFlowContainer.hpp"
#include "graphics/containers/ScrollableContainer.hpp"
#include "graphics/animations/ContainerAnimations.hpp"
#include "input/events/KeyEvent.hpp"
#include "input/events/MouseEvent.hpp"
#include <mutex>

GDF_NS_START
static geode::Ref<Game> s_instance;

bool Game::init() {
    if (!CCScene::init()) return false;

    /*TEST CODE ~ Remove after finished*/
    addListener<KeyEvent>([](KeyEvent* e){
      if (e->m_key == cocos2d::KEY_Escape && e->m_pressed) {
        cocos2d::CCDirector::get()->popScene();
      }
      return true;
    });

    auto flowContainer = FillFlowContainer::create(FillDirection::Horizontal);
    for (int i = 0; i < 5; i++) {
      auto us = Container::create();
      us->setSize({120,50});
      us->setBorderRadius(10);
      us->setBackgroundColor({255,255,255,255});
      flowContainer->addChild(us);
      us->addListener<MouseEvent>([us](MouseEvent* e){
        if (e->m_eventType == MouseEventType::MouseUp)
          us->runAction(cocos2d::CCSequence::createWithTwoActions(
            animations::TintTo::create(0, {255, 0, 0, 255}),
            animations::TintTo::create(0.5, {255, 255, 255, 255})
          ));
        else if (e->m_eventType == MouseEventType::Enter) {
          us->runAction(cocos2d::CCSequence::createWithTwoActions(
            animations::TintTo::create(0, {0, 255, 0, 255}),
            animations::TintTo::create(0.5, {255, 255, 255, 255})
          ));
        }
        return true;
      });
    }
    
    auto scroll = ScrollableContainer::create();
    scroll->addChild(flowContainer);
    scroll->setScrollDirection(ScrollableContainer::ScrollDirection::Horizontal);
    this->addChild(scroll);
    geode::queueInMainThread([flowContainer, scroll]{
      flowContainer->updateLayout();
      scroll->setContentSize({1920, flowContainer->getContentHeight()});
    });
    
		auto menu = cocos2d::CCMenu::create();
    auto returnButton = CCMenuItemSpriteExtra::create(
			cocos2d::CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(Game::yeah)
		);
		menu->addChild(returnButton);

    auto devtoolsButton = CCMenuItemSpriteExtra::create(
			cocos2d::CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
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
bool Game::doDispatchEvent(Event *event,
                           std::type_index type) {
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
      auto eventTarget = geode::cast::typeinfo_cast<Container *>(child);
      if (eventTarget == nullptr)
        continue;
      if (!eventTarget->doDispatchEvent(event, type)) {
        return false;
      }
    }
  }
  // idk i think this would keep the object around
  auto idk = eventRefHolder.data();
  return true;
};
GDF_NS_END 
