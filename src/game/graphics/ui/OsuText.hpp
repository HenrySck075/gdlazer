#pragma once

#include <Geode/Geode.hpp>
#include "../../../macro.h"
#include "../../../frameworks/graphics/containers/Container.hpp"

GDL_NS_START
using namespace geode::prelude;

enum class FontType {
  Regular,
  Bold,
  Italic,
  Light
};

/// Wrapper around CCLabelTTF to provide a text node with Container features
class OsuText : public frameworks::Container {
  CCLabelTTF* m_textNode;
public:
  static OsuText* create(const char* text, FontType font = FontType::Regular, float fontSize = 18, CCTextAlignment alignment = CCTextAlignment::kCCTextAlignmentLeft) {
    $create_class(OsuText, init, text, font, fontSize, alignment);
  }
  bool init(const char* text, FontType font, float fontSize = 18, CCTextAlignment alignment = CCTextAlignment::kCCTextAlignmentLeft);
  void setString(std::string string) {
    m_textNode->setString(string.c_str());
  }
};

CCLabelTTF* OsuTextF(const char* text, FontType fontType = FontType::Regular, float fontSize = 18, CCTextAlignment alignment = CCTextAlignment::kCCTextAlignmentLeft);
GDL_NS_END
/*
class OsuText : public CCLabelBMFont {
public:
  static OsuText* create(const char* text, FontType fontType) {
    $create_class(OsuText, initWithString, text, m[fontType]);
  };
};
*/
