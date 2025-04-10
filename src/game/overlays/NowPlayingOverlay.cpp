#include "NowPlayingOverlay.hpp"
#include "../graphics/OsuColor.hpp"
#include "../../frameworks/graphics/CCEase2.hpp"

GDL_NS_START
using namespace frameworks;

bool NowPlayingOverlay::init() {
  if (!OsuOverlayContainer::init()) return false;
  main->setAnchor(Anchor::TopRight);
  main->setAnchorPoint({1,1});
  //main->setPadding({3});
  main->setBorderRadius(5);
  main->setContentSize({c_playerWidth,c_playerHeight}, Unit::UIKit);
  main->setScale(0.9);
  main->setColor(OsuColor::YellowDarker);
  main->setOpacity(0);
  //name = "NowPlayingOverlay";
  return true;
}
void NowPlayingOverlay::onOpen() {
  stopAllActions();
  main->stopAllActions();
  main->runAction(
    CCSpawn::createWithTwoActions(
      easingsActions::CCEaseElasticOut::create(CCScaleTo::create(c_transitionLength, 1)),
      easingsActions::CCEaseOut::create(CCFadeTo::create(c_transitionLength, 255),5)
    )
  );
};
void NowPlayingOverlay::onClose() {
  stopAllActions();
  main->stopAllActions();
  main->runAction(
    CCSpawn::createWithTwoActions(
      easingsActions::CCEaseElasticOut::create(CCScaleTo::create(c_transitionLength, 0.9)),
      easingsActions::CCEaseOut::create(CCFadeTo::create(c_transitionLength, 0),5)
    )
  );
  runAction(CCDelayTime::create(c_transitionLength));
};
GDL_NS_END
