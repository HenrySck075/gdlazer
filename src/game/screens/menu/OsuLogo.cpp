#include "OsuLogo.hpp"
#include "../../graphics/CCResizableSprite.hpp"
#include "Geode/utils/general.hpp"
#include <cmath>

float lin2dB(float linear)
{
    return -(clamp(log10(linear) * 20.0f, -80.f, 1.0f));
}
bool OsuLogo::init() {
  ButtonBase::init(&OsuLogo::onClick);
  instance = BeatDetector::Instance();
  auto logoSprite = CCResizableSprite::createWithSpriteFrameName("logo.png"_spr);
  logoSprite->setID("m"_spr);
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
