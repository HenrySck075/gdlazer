#pragma once

#include <Geode/Geode.hpp>
#include "../../../macro.h"

GDL_NS_START
using namespace geode::prelude;

enum class FontType {
  Regular,
  Bold,
  Italic,
  Light
};


CCLabelTTF* OsuText(const char* text, FontType fontType = FontType::Regular, float fontSize = 18, CCTextAlignment alignment = CCTextAlignment::kCCTextAlignmentLeft);
GDL_NS_END
/*
class OsuText : public CCLabelBMFont {
public:
  static OsuText* create(const char* text, FontType fontType) {
    $create_class(OsuText, initWithString, text, m[fontType]);
  };
};
*/