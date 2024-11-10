#include "NowPlayingOverlay.hpp"
#include "../graphics/OsuColor.hpp"
#include <henrysck075.easings/include/easings.hpp>

bool NowPlayingOverlay::init() {
  if (!OsuOverlayContainer::init()) return false;
  main->setAnchor(Anchor::TopRight);
  main->setAnchorPoint({1,1});
  main->setPadding({3});
  main->setRadius(5);
  main->setContentSizeWithUnit({player_width,player_height}, Unit::UIKit, Unit::UIKit);
  main->setScale(0.9);
  main->setColor(OsuColor::YellowDarker);
  main->setOpacity(0);
  return true;
}
void NowPlayingOverlay::onOpen() {
  stopAllActions();
  main->stopAllActions();
  main->runAction(
    CCSpawn::createWithTwoActions(
      easingsActions::CCEaseElasticOut::create(CCScaleTo::create(transition_length, 1)),
      easingsActions::CCEaseOut::create(CCFadeTo::create(transition_length, 255),5)
    )
  );
};
void NowPlayingOverlay::onClose() {
  stopAllActions();
  main->stopAllActions();
  main->runAction(
    CCSpawn::createWithTwoActions(
      easingsActions::CCEaseElasticOut::create(CCScaleTo::create(transition_length, 0.9)),
      easingsActions::CCEaseOut::create(CCFadeTo::create(transition_length, 0),5)
    )
  );
  runAction(CCDelayTime::create(transition_length));
};