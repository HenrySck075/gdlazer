#include "OsuText.hpp"
// silly workaround but requires the font to be installed, which is not happening on mobile devices
std::map<FontType, const char*> fontTypeMap = {
    {FontType::Regular, "torus-regular.ttf"_spr },  
    {FontType::Bold, "torus-bold.ttf"_spr },
    {FontType::Italic, "torus-italic.ttf"_spr },
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