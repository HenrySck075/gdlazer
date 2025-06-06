#include "OsuText.hpp"
#define torusRegular "Torus-Regular.fnt"_spr
#define torusBold "Torus-Bold.fnt"_spr
#define torusItalic "Torus-Italic.fnt"_spr
#define torusLight "Torus-Light.fnt"_spr

/*
//#ifdef GEODE_IS_WINDOWS
#if 1
#define torusRegular (geode::Mod::get()->getResourcesDir() / "torus_regular.fnt").string()
#define torusBold (geode::Mod::get()->getResourcesDir() / "torus_bold.fnt").string()
#define torusItalic (geode::Mod::get()->getResourcesDir() / "torus_italic.fnt").string()
#define torusLight (geode::Mod::get()->getResourcesDir() / "torus_light.fnt").string()
#endif
*/
GDL_NS_START

static std::map<FontType, std::string> fontTypeMap = {
  {FontType::Regular, torusRegular },  
  {FontType::Bold, torusBold },
  {FontType::Italic, torusItalic },
  {FontType::Light, torusLight }
};

bool OsuText::init(std::string text, FontType font, float fontSize, CCTextAlignment alignment) {
  if (!frameworks::Container::init()) return false;
  auto fontFile = fontTypeMap[font];
  m_text = text;
  m_textNode = $verifyPtr(CCLabelBMFont::create(
    m_text.c_str(), 
    fontFile.c_str()
    //text.length()*fontSize/2
  ));
  m_textNode->setScale(processUnit(fontSize, frameworks::Unit::UIKit, false));
  m_textNode->setAlignment(alignment);
  m_textNode->setAnchorPoint({0.5,0.5});
  addChild(m_textNode);
  addListener<frameworks::NodeLayoutUpdated>([this](frameworks::NodeLayoutUpdated*){
    m_textNode->setPosition(getContentSize()/2);
    return true;
  });
  setContentSize(m_textNode->getScaledContentSize());
  return true;
};

void OsuText::setOpacity(GLubyte opacity) {
  frameworks::Container::setOpacity(opacity);
  m_textNode->setOpacity(opacity);
}

void OsuText::updateDisplayedOpacity(GLubyte parentOpacity) {
  frameworks::Container::updateDisplayedOpacity(parentOpacity);
  m_textNode->setOpacity(getDisplayedOpacity());
}
void OsuText::setString(std::string string) {
  m_text = string;
  m_textNode->setString(m_text.c_str());
}

GDL_NS_END
