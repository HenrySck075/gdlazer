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
  geode::Ref<CCLabelTTF> m_textNode;
  std::string m_text; // debugging purpose
public:
  static OsuText* create(std::string text, FontType font = FontType::Regular, float fontSize = 18, CCTextAlignment alignment = CCTextAlignment::kCCTextAlignmentLeft) {
    $create_class(OsuText, init, text, font, fontSize, alignment);
  }
  bool init(std::string text, FontType font, float fontSize = 18, CCTextAlignment alignment = CCTextAlignment::kCCTextAlignmentLeft);
  void setString(std::string string) {
    m_text = string;
    m_textNode->setString(m_text.c_str());
  }

  void setOpacity(GLubyte opacity) override;
  void updateDisplayedOpacity(GLubyte parentOpacity) override;
};

CCLabelTTF* OsuTextF(const std::string& text, FontType fontType = FontType::Regular, float fontSize = 18, CCTextAlignment alignment = CCTextAlignment::kCCTextAlignmentLeft);
GDL_NS_END
/*
class OsuText : public CCLabelBMFont {
public:
  static OsuText* create(const char* text, FontType fontType) {
    $create_class(OsuText, initWithString, text, m[fontType]);
  };
};
*/
