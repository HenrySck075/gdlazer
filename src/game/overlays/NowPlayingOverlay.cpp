#include "NowPlayingOverlay.hpp"
#include "../graphics/OsuColor.hpp"
#include <henrysck075.easings/include/easings.hpp>

bool NowPlayingOverlay::init() {
  if (!OsuOverlayContainer::init()) return false;
  main->setAnchor(Anchor::TopRight);
  main->setAnchorPoint({0.5,0.5});
  main->setPadding({3});
  main->setRadius(5);
  main->setContentSizeWithUnit({player_width,player_height}, Unit::UIKit, Unit::UIKit);
  main->setScale(0.9);
  main->setColor(OsuColor::YellowDarker);
  main->setOpacity(255);
  return true;
}
void NowPlayingOverlay::onOpen() {
  stopAllActions();
  main->runAction(easingsActions::CCEaseElasticOut::create(
    CCScaleTo::create(transition_length, 1)
  ));
};
void NowPlayingOverlay::onClose() {
  stopAllActions();
  main->runAction(easingsActions::CCEaseElasticOut::create(
    CCScaleTo::create(transition_length, 1)
  ));
};