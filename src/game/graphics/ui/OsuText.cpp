#include "OsuText.hpp"
#define torusRegular "torus_regular.ttf"_spr
#define torusBold "torus_bold.ttf"_spr
#define torusItalic "torus_italic.ttf"_spr
#define torusLight "torus_light.ttf"_spr

/*
//#ifdef GEODE_IS_WINDOWS
#if 1
#define torusRegular (geode::Mod::get()->getResourcesDir() / "torus_regular.ttf").string()
#define torusBold (geode::Mod::get()->getResourcesDir() / "torus_bold.ttf").string()
#define torusItalic (geode::Mod::get()->getResourcesDir() / "torus_italic.ttf").string()
#define torusLight (geode::Mod::get()->getResourcesDir() / "torus_light.ttf").string()
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
  geode::log::debug("[OsuText]: {}", fontFile);
  m_text = text;
  m_textNode = GDL_VALIDATE(CCLabelTTF::create(
    m_text.c_str(), 
    fontFile.c_str(),
    fontSize
  ));
  m_textNode->setHorizontalAlignment(alignment);
  m_textNode->setAnchorPoint({0.5,0.5});
  addChild(m_textNode);
  addListener<frameworks::NodeLayoutUpdated>([this](frameworks::NodeLayoutUpdated*){
    m_textNode->setPosition(getContentSize()/2);
    return true;
  });
  setContentSize(m_textNode->getContentSize());
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