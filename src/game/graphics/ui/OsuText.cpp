#include "OsuText.hpp"
std::map<FontType, const char*> fontTypeMap = {
    {FontType::Regular, "torus-regular.ttf"_spr },  
    {FontType::Bold, "torus-bold.ttf"_spr },
    {FontType::Italic, "torus-italic.ttf"_spr },
};
CCLabelTTF* OsuText(const char* text, FontType fontType, float fontSize) {
    auto fontFile = fontTypeMap[fontType];
    //log::debug("[OsuText]: Font path of {} is {}",fontFile,); 
    // the j
    return CCLabelTTF::create(text, CCFileUtils::sharedFileUtils()->fullPathForFilename(fontFile,true).c_str(), fontSize);
}