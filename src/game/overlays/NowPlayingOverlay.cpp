#include "NowPlayingOverlay.hpp"
#include "../graphics/OsuColor.hpp"
#include "../../frameworks/graphics/CCEase2.hpp"

GDL_NS_START
using namespace frameworks;

bool NowPlayingOverlay::init() {
  if (!OsuOverlayContainer::init()) return false;
  m_main->setAnchor(Anchor::TopRight);
  m_main->setAnchorPoint({1,1});
  //main->setPadding({3});
  m_main->setBorderRadius(5);
  m_main->setContentSize({c_playerWidth,c_playerHeight}, Unit::UIKit);
  m_main->setScale(0.9);
  m_main->setColor(OsuColor::YellowDarker);
  m_main->setOpacity(0);
  setName("NowPlayingOverlay");
  return true;
}
void NowPlayingOverlay::onOpen() {
  stopAllActions();
  m_main->stopAllActions();
  m_main->runAction(
    CCSpawn::createWithTwoActions(
      frameworks::ActionEase::create(CCScaleTo::create(c_transitionLength, 1), Easing::OutElastic),
      frameworks::ActionEase::create(CCFadeTo::create(c_transitionLength, 255),Easing::OutQuint)
    )
  );
};
void NowPlayingOverlay::onClose() {
  stopAllActions();
  m_main->stopAllActions();
  m_main->runAction(
    CCSpawn::createWithTwoActions(
      frameworks::ActionEase::create(CCScaleTo::create(c_transitionLength, 0.9), Easing::OutElastic),
      frameworks::ActionEase::create(CCFadeTo::create(c_transitionLength, 0), Easing::OutQuint)
    )
  );
  runAction(CCDelayTime::create(c_transitionLength));
};
GDL_NS_END
