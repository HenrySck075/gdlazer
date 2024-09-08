#include "OsuText.hpp"
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
    auto ret = CCLabelTTF::create(text, CCFileUtils::sharedFileUtils()->fullPathForFilename(fontFile,true).c_str(), fontSize);
    ret->setHorizontalAlignment(alignment);
    return ret;
}