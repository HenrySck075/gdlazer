#include "NowPlayingOverlay.hpp"
#include "../graphics/OsuColor.hpp"
#include "../../frameworks/graphics/animations/ActionEase.hpp"
#include "../../frameworks/graphics/animations/ContainerActions.hpp"

GDL_NS_START
using namespace frameworks;

bool NowPlayingOverlay::init() {
  if (!OsuOverlayContainer::init()) return false;
  m_main->setAnchor(Anchor::TopRight);
  m_main->setAnchorPoint({1,1});
  //main->setPadding({3});
  m_main->setBorderRadius(5);
  m_main->setClippingEnabled(true);
  m_main->setContentSize({c_playerWidth,c_playerHeight}, Unit::UIKit);
  m_main->setScale(0.9);
  m_main->setBackgroundColor(OsuColor::YellowDarker.opacity(0));
  setBackgroundColor({0,0,0,static_cast<GLubyte>(255*0.2)});
  setName("NowPlayingOverlay");

  m_ctrl = AudioManager::get();
  addListener<MusicStarted>([this](MusicStarted*) {
    m_currentSongDuration = m_ctrl->getSongDuration();
    return true;
  });

  m_bar = Container::create();
  m_bar->setContentHeight(2);
  m_bar->setBackgroundColor(OsuColor::Yellow);
  m_main->addChild(m_bar);

  scheduleUpdate();

  return true;
}
void NowPlayingOverlay::onOpen() {
  stopAllActions();
  m_currentSongDuration = m_ctrl->getSongDuration();
  m_main->stopAllActions();
  m_main->runAction(
    CCSpawn::createWithTwoActions(
      frameworks::ActionEase::create(CCScaleTo::create(c_transitionLength, 1), Easing::OutElastic),
      frameworks::ActionEase::create(ContainerTintOpacityTo::create(c_transitionLength, 255),Easing::OutQuint)
    )
  );
};
void NowPlayingOverlay::onClose() {
  stopAllActions();
  m_main->stopAllActions();
  m_main->runAction(
    CCSpawn::createWithTwoActions(
      frameworks::ActionEase::create(CCScaleTo::create(c_transitionLength, 0.9), Easing::OutQuint),
      frameworks::ActionEase::create(ContainerTintOpacityTo::create(c_transitionLength, 0), Easing::OutQuint)
    )
  );
  runAction(CCDelayTime::create(c_transitionLength));
};
void NowPlayingOverlay::update(float dt) {
  if (m_currentSongDuration == 0) {
    return;
  }
  m_bar->setContentWidth(m_ctrl->getElapsed()/m_currentSongDuration*100, Unit::Percent);
}
GDL_NS_END
