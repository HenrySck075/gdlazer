#pragma once

#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

CCSprite* extractSpriteFromLabel(CCLabelBMFont* label) {
	return CCSprite::createWithTexture(static_cast<CCSprite*>(label->getChildren()->objectAtIndex(0))->getTexture());
}

#define iconFont(name, charid) CCSprite* name = extractSpriteFromLabel(CCLabelBMFont::create(std::to_string(charid).c_str()))
#define icon(name, sprite) CCSprite* name = CCSprite::createWithSpriteFrameName("henrysck075.osulazer/icon-"##sprite)

struct OsuIcon {
   iconFont(Logo, "logo.png");
};

#undef icon
