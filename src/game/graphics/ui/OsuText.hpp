#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"

using namespace geode::prelude;

enum class FontType {
	Regular,
	Bold,
	Italic
};

std::map<FontType, const char*> m = {
	{FontType::Regular, "torus-regular.fnt"_spr },
	{FontType::Bold, "torus-bold.fnt"_spr },
	{FontType::Italic, "torus-italic.fnt"_spr },
};

#define OsuText(text, fontType) CCLabelBMFont::create(text, m[fontType])
/*
class OsuText : public CCLabelBMFont {
public:
	static OsuText* create(const char* text, FontType fontType) {
		create_class(OsuText, initWithString, text, m[fontType]);
	};
};
*/