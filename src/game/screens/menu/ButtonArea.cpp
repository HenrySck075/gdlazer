// compilers are happy, im not

#include "ButtonArea.hpp"
#include "../../../helpers/CustomActions.hpp"
#include "Geode/loader/Log.hpp"
#include <Geode/utils/ranges.hpp>
#include "ButtonConstants.hpp"
#include "../../graphics/OsuColor.hpp"
#include "../../../frameworks/graphics/CCEase2.hpp"
#include "../../../frameworks/graphics/containers/ContainerActions.hpp"

GDL_NS_START
using namespace frameworks;
bool ButtonArea::init(const CCPoint& anchorPos) {
  if (!Container::init()) return false;
  anchorPosition = anchorPos;
  setAnchorPoint({0.5,0.5});
  setAnchor(Anchor::Center);
  setContentSize({100,100},Unit::Percent);
  colorBg = CCLayerColor::create();
  colorBg->setColor(OsuColor::Gray(50));
  colorBg->ignoreAnchorPointForPosition(false);
  colorBg->setAnchorPoint({0.5,0.5});
  colorBg->setOpacity(255);
  colorBg->setZOrder(-77);
  addChild(colorBg);
  addListener<NodeLayoutUpdated>([this](NodeLayoutUpdated* e){
    auto the = CCNode::getContentSize();
    colorBg->setContentSize(the);
    colorBg->setPosition(the/2);
    return true;
  });

  return true;
}
/*
bool ButtonArea::doDispatchEvent(frameworks::Event *e, std::type_index i) {
  if (i == std::type_index(typeid(KeyEvent)))
    return Container::dispatchEvent(e);
  else {
    auto cur = getCurrent();
    if (cur.has_value()) {
      for (auto *child : geode::cocos::CCArrayExt<CCNode *>(
      getChildByIDRecursive("buttonarea_" + cur.value()) ->getChildren()
      )) {
        if (auto container = geode::cast::typeinfo_cast<Container *>(child)) {
      }
      };
    }
    return true;
  }
};
*/
void ButtonArea::constructButtons(CCArrayExt<MainMenuButton*> buttons, std::string tag) {
  auto b = CCLayer::create();
  CCPoint gap = {30,0};
  b->ignoreAnchorPointForPosition(true);
  b->setAnchorPoint({0,0.5});
  b->setPosition(anchorPosition+gap);
  b->setLayout(RowLayout::create()->setGap(-1)->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setAxisReverse(true));
  int buttonsCount = buttons.size();
  for (int i = 1; i<buttonsCount; i++) {
    auto item = buttons[i];
    if (item == nullptr) continue;
    b->addChild(item);
    item->setZOrder(buttonsCount-i);
  };
  b->updateLayout();
  b->setTag(2);
  
  auto t = CCLayer::create();
  t->ignoreAnchorPointForPosition(false);
  t->setAnchorPoint({1,0.5});
  t->setPosition(anchorPosition-gap);
  {
    auto leftChild = buttons[0];
    if (leftChild != nullptr) {
      t->addChild(leftChild);
      leftChild->setAnchorPoint({1,0.5});
    }
  }
  t->setLayout(RowLayout::create()
    ->setGap(-1)
    ->setAutoScale(false)
    ->setAxisAlignment(AxisAlignment::End)
    ->setAxisReverse(true)
  );
  t->updateLayout();
  t->setTag(1);

  // root node
  auto j = Container::create();
  j->addChild(t);
  j->addChild(b);
  j->setID("buttonarea_"+tag);
  j->setContentSize({100,100},Unit::Percent);
  j->setPosition({50,0},Unit::Percent,Unit::OpenGL);
  buttonsMenus[tag] = j;
  _buttons[tag] = buttons.inner();
  log::info("[ButtonArea]: Button menu \"{}\" registered.", tag);
}

void ButtonArea::show(std::string tag) {
  stopActionByTag(7777);
  auto _cur = getCurrent();
  int shownIndex, index;
  if (_cur.has_value()) {
  // just for comparison
    shownIndex = ranges::indexOf(tagsStack, [_cur](std::string t){return t==_cur;}).value_or(-1);
    index = ranges::indexOf(tagsStack, [tag](std::string t){return t==tag;}).value_or(-1);
    if (index==-1) {
      if (!buttonsMenus.contains(tag)) {
        log::error("[ButtonArea]: \"{}\" is not a registered menu.", tag);
        return;
      }
      index = tagsStack.size();
    }
    if (shownIndex != -1) hide(_cur.value(), shownIndex>index);
    if (shownIndex==index) return;
  } else {
    shownIndex = 0;
    index = 1;
  }

  if (hidden) {
	  hidden = false;
    colorBg->runAction(CCEaseSineIn::create(CCScaleTo::create(0.3,1,1)));
  }

  CCArrayExt<MainMenuButton*> j = _buttons[tag].operator->();
  if (buttonsMenus[tag]->getParent() == nullptr) 
    addChild(buttonsMenus[tag]);

  // expand
  if (shownIndex<index) {
    tagsStack.push_back(tag);
    for (int idx = 0; idx<j.size(); idx++) {
      auto i = j[idx];
      i->stopAllActions();
      auto pos = i->getPosition();
      i->runAction(CCSequence::createWithTwoActions(
        frameworks::ActionEase::create(CCSpawn::createWithTwoActions(
          CCMoveFromTo::create(animationSpeed, CCPoint(idx!=0?-15:pos.x+15,i->getPositionY()), pos),
          CCFadeIn::create(animationSpeed)
        ), Easing::OutExpo),
        CCCallFuncL::create([i](){
          auto nugget = i;
          nugget->setMouseEnabled(true);
          nugget->askForUpdate(true);
        })
      ));
    }
    buttonsMenus[tag]->setZOrder(curZOrder--);
  }
  // return
  else {
    for (int idx = 0; idx<j.size(); idx++) {
      auto i = j[idx];
      i->stopAllActions();
      auto pos = i->getPosition();
      i->runAction(CCSequence::createWithTwoActions(
        frameworks::ActionEase::create(CCSpawn::createWithTwoActions(
          ContainerResizeTo::create(animationSpeed, BUTTON_WIDTH, c_buttonAreaHeight),
          CCFadeIn::create(animationSpeed)
        ), Easing::OutExpo),
        CCCallFuncL::create([i](){
          auto nugget = i;
          nugget->setMouseEnabled(true);
          nugget->askForUpdate(true);
        })
      ));
    }
    // the node is still there so we dont need to add them
    // however
    CCPoint gap = {20,0};

    auto btnLayer = buttonsMenus[tag];
    auto btnL1 = btnLayer->getChildByTag(1);
    auto btnL2 = btnLayer->getChildByTag(2);
    btnL1->runAction(frameworks::ActionEase::create(
      CCMoveTo::create(animationSpeed, anchorPosition-gap)
    , Easing::OutExpo));
    btnL2->runAction(frameworks::ActionEase::create(
      CCMoveTo::create(animationSpeed, anchorPosition+gap)
    , Easing::OutExpo));

    while (tagsStack.back()!=tag) {tagsStack.pop_back();}
  }
}

void ButtonArea::hide(std::string tag, bool collapse, bool close) {
  if (buttonsMenus.contains(tag)) {
    CCArrayExt<MainMenuButton*> j = _buttons[tag].operator->();
    if (close) {
      colorBg->runAction(frameworks::ActionEase::create(CCScaleTo::create(0.3,1,0), Easing::OutQuint));
      hidden = true;
    }
    if (collapse) {
      for (int idx = 0; idx<j.size(); idx++) {
        auto i = j[idx];
        i->setMouseEnabled(false);
        i->stopAllActions();
        auto pos = i->getPosition();
        i->runAction(
          frameworks::ActionEase::create(
            CCSpawn::createWithTwoActions(
              CCMoveTo::create(animationSpeed, {idx!=0?-15:pos.x+15,pos.y}),
              CCFadeOut::create(animationSpeed)
            )
          , Easing::OutExpo)
        );
      }
      curZOrder--;
    } else {
      for (int idx = 0; idx<j.size(); idx++) {
        auto i = j[idx];
        i->setMouseEnabled(false);
        i->stopAllActions();
        auto pos = i->getPosition();
        i->runAction(
          frameworks::ActionEase::create(
            CCSpawn::createWithTwoActions(
              ContainerResizeTo::create(animationSpeed, BUTTON_WIDTH*1.5,c_buttonAreaHeight),
              CCFadeOut::create(animationSpeed)
            )
          , Easing::OutExpo)
        );
      };
      auto btnLayer = buttonsMenus[tag];
      auto btnL1 = btnLayer->getChildByTag(1);
      auto btnL2 = btnLayer->getChildByTag(2);
      auto w = CCDirector::sharedDirector()->getWinSize().width;
      btnL1->runAction(frameworks::ActionEase::create(
        CCMoveTo::create(animationSpeed, ccp(-w/2,btnL1->getPositionY()))
      , Easing::OutExpo));
      btnL2->runAction(frameworks::ActionEase::create(
        CCMoveTo::create(animationSpeed, ccp(w/2,btnL2->getPositionY()))
      , Easing::OutExpo));
    }
    runAction(
      CCSequence::createWithTwoActions(
        CCDelayTime::create(animationSpeed),
        CCCallFuncL::create([this,tag](){
          buttonsMenus[tag]->getChildByTag(1)->updateLayout();
          buttonsMenus[tag]->getChildByTag(2)->updateLayout();
          removeChild(buttonsMenus[tag]);
        })
      )
    )->setTag(7777);
  }
}
GDL_NS_END
