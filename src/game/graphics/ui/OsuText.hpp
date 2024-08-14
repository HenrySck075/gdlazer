#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"

using namespace geode::prelude;

enum class FontType {
    Regular,
    Bold,
    Italic
};

std::map<FontType, const char*> fontTypeMap = {
    {FontType::Regular, "torus-regular.fnt"_spr },
    {FontType::Bold, "torus-bold.fnt"_spr },
    {FontType::Italic, "torus-italic.fnt"_spr },
};

CCLabelBMFont* OsuText(const char* text, FontType fontType) {return CCLabelBMFont::create(text, fontTypeMap[fontType]);}
/*
class OsuText : public CCLabelBMFont {
public:
    static OsuText* create(const char* text, FontType fontType) {
        create_class(OsuText, initWithString, text, m[fontType]);
    };
};
*/