#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"

using namespace geode::prelude;

enum class FontType {
    Regular,
    Bold,
    Italic
};


CCLabelTTF* OsuText(const char* text, FontType fontType, float fontSize = 18);
/*
class OsuText : public CCLabelBMFont {
public:
    static OsuText* create(const char* text, FontType fontType) {
        create_class(OsuText, initWithString, text, m[fontType]);
    };
};
*/