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
  m_fontSize = fontSize;
  auto fontFile = fontTypeMap[font];
  m_text = text;
  m_textNode = $verifyPtr(CCLabelBMFont::create(
    m_text.c_str(), 
    fontFile.c_str()
    //text.length()*fontSize/2
  ));
  if (text.size() != 0) {
    m_textNode->setScale(fontSize/42/cocos2d::CCDirector::get()->getOpenGLView()->getFrameZoomFactor());
    setContentSize(m_textNode->getScaledContentSize());
  }
  m_textNode->setAlignment(alignment);
  m_textNode->setAnchorPoint({0.5,0.5});
  addChild(m_textNode);
  addListener<frameworks::NodeLayoutUpdated>([this](frameworks::NodeLayoutUpdated*){
    m_textNode->setPosition(getContentSize()/2);
    return true;
  });
  return true;
};

void OsuText::setKerning(float kerning) {
  m_textNode->setExtraKerning(kerning);
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
  bool unemptied = string.size() != 0 && m_text.size() == 0;
  m_text = string;
  m_textNode->setString(m_text.c_str());
  if (unemptied) {
    m_textNode->setScale(
      m_fontSize/42/cocos2d::CCDirector::get()->getOpenGLView()->getFrameZoomFactor()
      /*processUnit(m_fontSize, frameworks::Unit::UIKit, false) / m_textNode->getContentHeight()*/
    );
    setContentSize(m_textNode->getScaledContentSize());
  }
  //setContentSize(m_textNode->getScaledContentSize());
}

GDL_NS_END
