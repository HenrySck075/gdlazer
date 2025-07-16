#include "PlaygroundScreen.hpp"

#include "Geode/loader/Loader.hpp"
#include "../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../frameworks/graphics/containers/ScrollContainer.hpp"
#include "../../frameworks/graphics/animations/ContainerAnimations.hpp"
#include "../../frameworks/input/events/MouseEvent.hpp"
#include "../../frameworks/Game.hpp"

using namespace gdlazer::frameworks;

GDL_NS_START

bool PlaygroundScreen::init() {
  if (!Screen::init())
    return false;

  auto flowContainer = FillFlowContainer::create({FillDirection::Horizontal});
  for (int i = 0; i < 5; i++) {
    auto us = Container::create();
    us->setContentSize({120, 50});
    us->setBorderRadius(10 + i * 5);
    us->setBackgroundColor({255, 255, 255, 255});
    flowContainer->addChild(us);
    us->addListener<MouseEvent>([us](MouseEvent *e) {
      if (e->m_eventType == MouseEventType::MouseUp)
        us->runAction(cocos2d::CCSequence::createWithTwoActions(
            animations::TintTo::create(0, {255, 0, 0, 255}),
            animations::TintTo::create(0.5, {255, 255, 255, 255})));
      else if (e->m_eventType == MouseEventType::Enter) {
        us->runAction(cocos2d::CCSequence::createWithTwoActions(
            animations::TintTo::create(0, {0, 255, 0, 255}),
            animations::TintTo::create(0.5, {255, 255, 255, 255})));
      }
      return true;
    });
  }

  auto scroll = ScrollContainer::create(flowContainer);
  scroll->setScrollDirection(ScrollDirection::Horizontal);
  this->addChild(scroll);
  geode::queueInMainThread([flowContainer, scroll] {
    flowContainer->updateLayout();
    scroll->setContentSize({1920, flowContainer->getContentHeight()});
  });

  auto menu = cocos2d::CCMenu::create();
  auto returnButton = CCMenuItemSpriteExtra::create(
      cocos2d::CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"), this,
      menu_selector(Game::yeah));
  menu->addChild(returnButton);

  auto devtoolsButton = CCMenuItemSpriteExtra::create(
      cocos2d::CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"), this,
      menu_selector(Game::weh));
  menu->addChild(devtoolsButton);
  devtoolsButton->setPosition({200, 0});

  this->addChild(menu);
  return true;
};

GDL_NS_END
