#include "OsuLogo.hpp"
#include "../../graphics/CCResizableSprite.hpp"



bool OsuLogo::init() {
  BeatSyncedContainer::init();
  ButtonBase::init(OsuLogo::onClick);
  auto logoSprite = CCResizableSprite::createWithSpriteFrameName("logo.png"_spr);
  this->ButtonBase::setContentSize(logoSprite->getContentSize());

  return true;
}
