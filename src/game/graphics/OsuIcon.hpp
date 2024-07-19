#include "CCResizableSprite.hpp"

#define icon(name, spriteName) CCResizableSprite* name = CCResizableSprite::createWithSpriteFrameName(spriteName)

struct OsuIcon {
   icon(Settings, "osuicons/settings.png"_spr);
};

#undef icon
