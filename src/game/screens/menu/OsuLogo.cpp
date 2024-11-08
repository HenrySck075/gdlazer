#include "OsuLogo.hpp"
#include "../../../framework/graphics/sprites/CCResizableSprite.hpp"
#include <cmath>
#include <numeric>

float lin2dB(float linear)
{
  return -(clamp(log10(linear) * 20.0f, -80.f, 1.0f));
}
bool OsuLogo::init() {
  ClickableContainer::init("osu-logo-select.wav"_spr);
  /*
  m_listener = this->template addEventListener<EventFilter<BeatEvent>>([this](float elapsed) {
    const char* sfxName;
    if (beatCount == 3) {
      sfxName = "osu-logo-heartbeat.wav"_spr;
    }
    else {
      sfxName = "osu-logo-downbeat.wav"_spr;
    }
    FMODAudioEngine::sharedEngine()->playEffect(sfxName);
    beatCount++;
    if (beatCount == 4) { beatCount = 0; };
    return ListenerResult::Propagate;
  });
  */
  auto logoSprite = CCSprite::createWithSpriteFrameName("logo.png"_spr);
  this->setAnchorPoint({0.5, 0.5});

  if (logoSprite != nullptr) {
    logoSprite->setID("m");
    this->ClickableContainer::setContentSize(logoSprite->getContentSize());
    logoSprite->setPosition(logoSprite->getContentSize() / 2);
    logoSprite->setScale(0.6);
    addChild(logoSprite);
  }
  audio = AudioManager::get();

  // run update
  scheduleUpdate();
  return true;
}
void OsuLogo::update(float delta) {
  CCNode::update(delta);
  float dominantVol = 0;
  // currently always return 0
  audio->getDSP()->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &dominantVol,nullptr,0);
  log::debug("[OsuLogo]: {}", dominantVol);
  setScale(0.9+(dominantVol/2));
  /*
  auto spectrum = instance->getCurrentSpectrum();
  auto ss = instance->getSampleSize();
  float vol = 0;
  // todo: average
  for (int i = 0; i<ss; i++) {
    float m = spectrum[i];
    if (m>vol) vol=m;
  }
  float sc = lin2dB(vol);
  
  static_cast<CCResizableSprite*>(this->getChildByID("m"))->setScale(0.8+sc/40/10);
  */
}
/*
void OsuLogo::onBeat(float delta) {
  auto spr = static_cast<CCSprite*>(this->getChildByID("m"));
  spr->setScale(1);
  spr->runAction(CCScaleTo::create(0.5, 0.8));
}
*/
