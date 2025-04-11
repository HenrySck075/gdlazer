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
      easingsActions::CCEaseElasticOut::create(CCScaleTo::create(c_transitionLength, 1)),
      easingsActions::CCEaseOut::create(CCFadeTo::create(c_transitionLength, 255),5)
    )
  );
};
void NowPlayingOverlay::onClose() {
  stopAllActions();
  m_main->stopAllActions();
  m_main->runAction(
    CCSpawn::createWithTwoActions(
      easingsActions::CCEaseElasticOut::create(CCScaleTo::create(c_transitionLength, 0.9)),
      easingsActions::CCEaseOut::create(CCFadeTo::create(c_transitionLength, 0),5)
    )
  );
  runAction(CCDelayTime::create(c_transitionLength));
};
GDL_NS_END
