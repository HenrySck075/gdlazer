#include "OsuText.hpp"
std::map<FontType, const char*> fontTypeMap = {
    {FontType::Regular, "torus-regular.fnt"_spr },
    {FontType::Bold, "torus-bold.fnt"_spr },
    {FontType::Italic, "torus-italic.fnt"_spr },
};
CCLabelBMFont* OsuText(const char* text, FontType fontType){return CCLabelBMFont::create(text, fontTypeMap[fontType]);}