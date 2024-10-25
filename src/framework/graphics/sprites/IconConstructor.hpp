#pragma once

#include <string>
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
#include <Geode/loader/Log.hpp>
#include "CCResizableSprite.hpp"
std::string unicode_to_utf8(int unicode);

// j
struct IconConstructor {
    std::string glyphChar;
    int unicode;
    std::string fontName;

    IconConstructor(int unicode, std::string font) : unicode(unicode), glyphChar(unicode_to_utf8(unicode)), fontName(font) {};

    operator CCResizableSprite*();
};