#include "OsuText.hpp"
#ifdef amongus
  #define torusRegular "Torus Pro Regular"
  #define torusBold "Torus Pro Bold"
  #define torusItalic "Torus Pro Italic"
  #define torusLight "Torus Pro Light"
#else 
  #define torusRegular "torus-regular.ttf"_spr
  #define torusBold "torus-bold.ttf"_spr
  #define torusItalic "torus-italic.ttf"_spr
  #define torusLight "torus-light.ttf"_spr
#endif
// silly workaround for windows
static std::map<FontType, const char*> fontTypeMap = {
  {FontType::Regular, torusRegular },  
  {FontType::Bold, torusBold },
  {FontType::Italic, torusItalic },
  {FontType::Light, torusLight }
};

// todo: figure out why custom ttf isnt loaded in
CCLabelTTF* OsuText(const char* text, FontType fontType, float fontSize, CCTextAlignment alignment) {
  auto fontFile = fontTypeMap[fontType];
  //log::debug("[OsuText]: Font path of {} is {}",fontFile,); 
  // the j
  auto ret = CCLabelTTF::create(text, fontFile, fontSize);
  ret->setHorizontalAlignment(alignment);
  return ret;
}