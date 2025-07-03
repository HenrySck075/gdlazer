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

/// Wrapper around CCLabelBMFont to provide a text node with Container features
class OsuText : public frameworks::Container {
  struct ConstructorKwargs {
    std::string text;
    FontType font = FontType::Regular;
    float fontSize = 18;
    CCTextAlignment alignment = CCTextAlignment::kCCTextAlignmentLeft;
  };
public:
  static OsuText* create(GDF_KWARGS) {
    $createClass(OsuText, init, args);
  }
  bool init(GDF_KWARGS);
  void setString(std::string string);
  void setFontType(FontType type);

  geode::Ref<CCLabelBMFont> inner() {return m_textNode;}

  void setOpacity(GLubyte opacity) override;
  void updateDisplayedOpacity(GLubyte parentOpacity) override;
  void setKerning(float kerning);
private:
  geode::Ref<CCLabelBMFont> m_textNode;
  std::string m_text; // debugging purpose
  float m_fontSize;
  void resizeTextNode();
};

GDL_NS_END
