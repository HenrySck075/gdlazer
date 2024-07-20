#include "OsuLogo.hpp"
#include "../../graphics/CCResizableSprite.hpp"
#include "Geode/utils/general.hpp"
#include <cmath>

float lin2dB(float linear)
{
    return -(clamp(log10(linear) * 20.0f, -80.f, 1.0f));
}
bool OsuLogo::init() {
    ButtonBase::init([this](CCNode* idk) {
        FMODAudioEngine::sharedEngine()->playEffect("osu-logo-select.wav"_spr);
    },this);
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
  instance = BeatDetector::Instance();
  auto logoSprite = CCSprite::createWithSpriteFrameName("logo.png"_spr);
  logoSprite->setID("m");
  this->ButtonBase::setContentSize(logoSprite->getContentSize());
  logoSprite->setPosition(logoSprite->getContentSize()/2);
  this->addChild(logoSprite);

  return true;
}
void OsuLogo::update(float delta) {
  CCNode::update(delta);
  auto spectrum = instance->getCurrentSpectrum();
  auto ss = instance->getSampleSize();
  float vol = 0;
  // todo: average
  for (int i = 0; i<ss; i++) {
    float m = spectrum[i];
    if (m>vol) vol=m;
  }
  float sc = lin2dB(vol);
  
  static_cast<CCResizableSprite*>(this->getChildByID("m"_spr))->setScale(1+sc/40);
}