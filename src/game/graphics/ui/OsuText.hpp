#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"

using namespace geode::prelude;

enum class FontType {
	Regular,
	Bold,
	Italic
};

std::map<FontType, gd::string> m = {
	{FontType::Regular, "torus.fnt"_spr },
	{FontType::Bold, "torus-bold.fnt"_spr },
	{FontType::Italic, "torus-italic.fnt"_spr },
};

class OsuText : public CCLabelBMFont {
public:
	static OsuText* create(const char* text, FontType fontType) {
		create_class(OsuText, initWithString, text, m[fontType]);
	};
};