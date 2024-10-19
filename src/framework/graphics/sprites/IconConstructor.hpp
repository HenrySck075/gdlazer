#include <string>
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
#include <Geode/loader/Log.hpp>
std::string unicode_to_utf8(int unicode);

// j
struct IconConstructor {
    std::string glyphChar;
    std::string fontName;

    IconConstructor(std::string glyph, std::string font) : glyphChar(glyph), fontName(font) {};

    operator cocos2d::CCLabelBMFont*() {
        const char* g = glyphChar.c_str();
        const char* f = fontName.c_str();
        cocos2d::CCLabelBMFont* ret = cocos2d::CCLabelBMFont::create(g, f);
        if (!ret) {
            geode::log::error("[IconConstructor]: Icon does not exists on font {}", fontName);
            delete ret;
            ret = nullptr;
        }
        return ret;
    }
};