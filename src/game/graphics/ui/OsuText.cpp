#include "OsuText.hpp"
#define torusRegular "torus-regular.ttf"_spr
#define torusBold "torus-bold.ttf"_spr
#define torusItalic "torus-italic.ttf"_spr
#define torusLight "torus-light.ttf"_spr

#ifdef GEODE_IS_WINDOWS
#define torusRegular (geode::Mod::get()->getResourcesDir() / geode::Mod::get()->getID() / "torus-regular.ttf").string()
#define torusBold (geode::Mod::get()->getResourcesDir() / geode::Mod::get()->getID() / "torus-bold.ttf").string()
#define torusItalic (geode::Mod::get()->getResourcesDir() / geode::Mod::get()->getID() / "torus-italic.ttf").string()
#define torusLight (geode::Mod::get()->getResourcesDir() / geode::Mod::get()->getID() / "torus-light.ttf").string()
#endif

GDL_NS_START

static std::map<FontType, std::string> fontTypeMap = {
  {FontType::Regular, torusRegular },  
  {FontType::Bold, torusBold },
  {FontType::Italic, torusItalic },
  {FontType::Light, torusLight }
};

// todo: figure out why custom ttf isnt loaded in
CCLabelTTF* OsuTextF(const std::string& text, FontType fontType, float fontSize, CCTextAlignment alignment) {
  auto fontFile = fontTypeMap[fontType];
  //log::debug("[OsuText]: Font path of {} is {}",fontFile,); 
  // the j
  auto ret = CCLabelTTF::create(
    text.c_str(), 
    #ifdef GEODE_IS_WINDOWS
    (geode::Mod::get()->getResourcesDir() / fontFile).string().c_str(), 
    #else
    fontFile,
    #endif
    fontSize
  );
  if (ret == nullptr) {
    return nullptr;
  }
  ret->setHorizontalAlignment(alignment);
  return ret;
}

bool OsuText::init(std::string text, FontType font, float fontSize, CCTextAlignment alignment) {
  if (!frameworks::Container::init()) return false;
  m_textNode = OsuTextF((m_text = text), font, fontSize, alignment);
  if (m_textNode == nullptr) return false;
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
GDL_NS_END
