#include "OsuText.hpp"
#ifdef GEODE_IS_WINDOWS
#define torusRegular "torus-regular.ttf"_spr
#define torusBold "torus-bold.ttf"_spr
#define torusItalic "torus-italic.ttf"_spr
#define torusLight "torus-light.ttf"_spr
#else
#define torusRegular "torus-regular.ttf"
#define torusBold "torus-bold.ttf"
#define torusItalic "torus-italic.ttf"
#define torusLight "torus-light.ttf"
#endif
GDL_NS_START

static std::map<FontType, const char*> fontTypeMap = {
  {FontType::Regular, torusRegular },  
  {FontType::Bold, torusBold },
  {FontType::Italic, torusItalic },
  {FontType::Light, torusLight }
};

// todo: figure out why custom ttf isnt loaded in
CCLabelTTF* OsuTextF(const char* text, FontType fontType, float fontSize, CCTextAlignment alignment) {
  auto fontFile = fontTypeMap[fontType];
  //log::debug("[OsuText]: Font path of {} is {}",fontFile,); 
  // the j
  auto ret = CCLabelTTF::create(
    text, 
    #ifdef GEODE_IS_WINDOWS
    (geode::Mod::get()->getResourcesDir() / fontFile).string().c_str(), 
    #else
    fontFile,
    #endif
    fontSize
  );
  ret->setHorizontalAlignment(alignment);
  return ret;
}

bool OsuText::init(const char* text, FontType font, float fontSize, CCTextAlignment alignment) {
  if (!frameworks::Container::init()) return false;
  m_textNode = OsuTextF(text, font, fontSize, alignment);
  addChild(m_textNode);
  return true;
};
GDL_NS_END
