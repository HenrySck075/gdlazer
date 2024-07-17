#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;
// the 0-255 range one
#include <algorithm>
#include <cstdio>
struct Color4 {
  int r;
  int g;
  int b;
  int a;

  Color4 clamp() {
#define c(v) v=v<0?0:(v>255?255:v)
    c(r);
    c(g);
    c(b);
    c(a);
    return {r,g,b,a};
  }
  Color4 lighten(int amount) {
    int scalar = std::max(255,255+amount);
    return Color4{r*scalar,g*scalar,b*scalar,a}.clamp();
  }
  Color4 darken(int amount) {
    int scalar = std::max(255,255+amount);
    return Color4{r/scalar,g/scalar,b/scalar,a}.clamp();
  }

  // converters

  static Color4 fromHex(std::string hex) {
    int r,g,b,a;
    int c = hex.length();
    switch (c) {
      case 3:
        sscanf(hex.c_str(), "%01x%01x%01x", &r,&g,&b);
      case 4:
        if (hex.c_str()[0]=='#') sscanf(hex.c_str(), "#%01x%01x%01x", &r,&g,&b);
        else sscanf(hex.c_str(), "%01x%01x%01x%01x", &r,&g,&b,&a);
      case 5:
        sscanf(hex.c_str(), "#%01x%01x%01x%01x", &r,&g,&b,&a);
      case 6:
        sscanf(hex.c_str(), "%02x%02x%02x", &r,&g,&b);
      case 7:
        sscanf(hex.c_str(), "#%02x%02x%02x", &r,&g,&b);
      case 8:
        sscanf(hex.c_str(), "%02x%02x%02x%02x", &r,&g,&b,&a);
      case 9:
        sscanf(hex.c_str(), "#%02x%02x%02x%02x", &r,&g,&b,&a);
    }
    if (c<5) {
      r*=17;
      g*=17;
      b*=17;
    }
    if (
      c==3 ||
      (c==4 && hex.c_str()[0]=='#' ) ||
      c==5 ||
      c==6
    ){
      a=255;
    }

    return {r,g,b,a};
  }


  // convert to cocos2d color because we're making this for cocos2d ofc
  operator ccColor4B() const {
    return ccc4(r,g,b,a);
  }
  operator ccColor4F() const {
    return ccc4f(r/255,g/255,b/255,a/255);
  }
};
struct Color4Defined {
    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 255, 255, 0).
    /// </summary>
  Color4 Transparent = Color4{255, 255, 255, 0};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (240, 248, 255, 255).
    /// </summary>
  Color4 AliceBlue = Color4{240, 248, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (250, 235, 215, 255).
    /// </summary>
  Color4 AntiqueWhite = Color4{250, 235, 215, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 255, 255, 255).
    /// </summary>
  Color4 Aqua = Color4{0, 255, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (127, 255, 212, 255).
    /// </summary>
  Color4 Aquamarine = Color4{127, 255, 212, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (240, 255, 255, 255).
    /// </summary>
  Color4 Azure = Color4{240, 255, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (245, 245, 220, 255).
    /// </summary>
  Color4 Beige = Color4{245, 245, 220, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 228, 196, 255).
    /// </summary>
  Color4 Bisque = Color4{255, 228, 196, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 0, 0, 255).
    /// </summary>
  Color4 Black = Color4{0, 0, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 235, 205, 255).
    /// </summary>
  Color4 BlanchedAlmond = Color4{255, 235, 205, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 0, 255, 255).
    /// </summary>
  Color4 Blue = Color4{0, 0, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (138, 43, 226, 255).
    /// </summary>
  Color4 BlueViolet = Color4{138, 43, 226, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (165, 42, 42, 255).
    /// </summary>
  Color4 Brown = Color4{165, 42, 42, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (222, 184, 135, 255).
    /// </summary>
  Color4 BurlyWood = Color4{222, 184, 135, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (95, 158, 160, 255).
    /// </summary>
  Color4 CadetBlue = Color4{95, 158, 160, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (127, 255, 0, 255).
    /// </summary>
  Color4 Chartreuse = Color4{127, 255, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (210, 105, 30, 255).
    /// </summary>
  Color4 Chocolate = Color4{210, 105, 30, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 127, 80, 255).
    /// </summary>
  Color4 Coral = Color4{255, 127, 80, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (100, 149, 237, 255).
    /// </summary>
  Color4 CornflowerBlue = Color4{100, 149, 237, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 248, 220, 255).
    /// </summary>
  Color4 Cornsilk = Color4{255, 248, 220, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (220, 20, 60, 255).
    /// </summary>
  Color4 Crimson = Color4{220, 20, 60, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 255, 255, 255).
    /// </summary>
  Color4 Cyan = Color4{0, 255, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 0, 139, 255).
    /// </summary>
  Color4 DarkBlue = Color4{0, 0, 139, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 139, 139, 255).
    /// </summary>
  Color4 DarkCyan = Color4{0, 139, 139, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (184, 134, 11, 255).
    /// </summary>
  Color4 DarkGoldenrod = Color4{184, 134, 11, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (169, 169, 169, 255).
    /// </summary>
  Color4 DarkGray = Color4{169, 169, 169, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 100, 0, 255).
    /// </summary>
  Color4 DarkGreen = Color4{0, 100, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (189, 183, 107, 255).
    /// </summary>
  Color4 DarkKhaki = Color4{189, 183, 107, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (139, 0, 139, 255).
    /// </summary>
  Color4 DarkMagenta = Color4{139, 0, 139, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (85, 107, 47, 255).
    /// </summary>
  Color4 DarkOliveGreen = Color4{85, 107, 47, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 140, 0, 255).
    /// </summary>
  Color4 DarkOrange = Color4{255, 140, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (153, 50, 204, 255).
    /// </summary>
  Color4 DarkOrchid = Color4{153, 50, 204, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (139, 0, 0, 255).
    /// </summary>
  Color4 DarkRed = Color4{139, 0, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (233, 150, 122, 255).
    /// </summary>
  Color4 DarkSalmon = Color4{233, 150, 122, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (143, 188, 139, 255).
    /// </summary>
  Color4 DarkSeaGreen = Color4{143, 188, 139, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (72, 61, 139, 255).
    /// </summary>
  Color4 DarkSlateBlue = Color4{72, 61, 139, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (47, 79, 79, 255).
    /// </summary>
  Color4 DarkSlateGray = Color4{47, 79, 79, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 206, 209, 255).
    /// </summary>
  Color4 DarkTurquoise = Color4{0, 206, 209, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (148, 0, 211, 255).
    /// </summary>
  Color4 DarkViolet = Color4{148, 0, 211, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 20, 147, 255).
    /// </summary>
  Color4 DeepPink = Color4{255, 20, 147, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 191, 255, 255).
    /// </summary>
  Color4 DeepSkyBlue = Color4{0, 191, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (105, 105, 105, 255).
    /// </summary>
  Color4 DimGray = Color4{105, 105, 105, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (30, 144, 255, 255).
    /// </summary>
  Color4 DodgerBlue = Color4{30, 144, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (178, 34, 34, 255).
    /// </summary>
  Color4 Firebrick = Color4{178, 34, 34, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 250, 240, 255).
    /// </summary>
  Color4 FloralWhite = Color4{255, 250, 240, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (34, 139, 34, 255).
    /// </summary>
  Color4 ForestGreen = Color4{34, 139, 34, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 0, 255, 255).
    /// </summary>
  Color4 Fuchsia = Color4{255, 0, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (220, 220, 220, 255).
    /// </summary>
  Color4 Gainsboro = Color4{220, 220, 220, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (248, 248, 255, 255).
    /// </summary>
  Color4 GhostWhite = Color4{248, 248, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 215, 0, 255).
    /// </summary>
  Color4 Gold = Color4{255, 215, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (218, 165, 32, 255).
    /// </summary>
  Color4 Goldenrod = Color4{218, 165, 32, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (128, 128, 128, 255).
    /// </summary>
  Color4 Gray = Color4{128, 128, 128, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 128, 0, 255).
    /// </summary>
  Color4 Green = Color4{0, 128, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (173, 255, 47, 255).
    /// </summary>
  Color4 GreenYellow = Color4{173, 255, 47, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (240, 255, 240, 255).
    /// </summary>
  Color4 Honeydew = Color4{240, 255, 240, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 105, 180, 255).
    /// </summary>
  Color4 HotPink = Color4{255, 105, 180, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (205, 92, 92, 255).
    /// </summary>
  Color4 IndianRed = Color4{205, 92, 92, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (75, 0, 130, 255).
    /// </summary>
  Color4 Indigo = Color4{75, 0, 130, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 255, 240, 255).
    /// </summary>
  Color4 Ivory = Color4{255, 255, 240, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (240, 230, 140, 255).
    /// </summary>
  Color4 Khaki = Color4{240, 230, 140, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (230, 230, 250, 255).
    /// </summary>
  Color4 Lavender = Color4{230, 230, 250, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 240, 245, 255).
    /// </summary>
  Color4 LavenderBlush = Color4{255, 240, 245, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (124, 252, 0, 255).
    /// </summary>
  Color4 LawnGreen = Color4{124, 252, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 250, 205, 255).
    /// </summary>
  Color4 LemonChiffon = Color4{255, 250, 205, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (173, 216, 230, 255).
    /// </summary>
  Color4 LightBlue = Color4{173, 216, 230, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (240, 128, 128, 255).
    /// </summary>
  Color4 LightCoral = Color4{240, 128, 128, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (224, 255, 255, 255).
    /// </summary>
  Color4 LightCyan = Color4{224, 255, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (250, 250, 210, 255).
    /// </summary>
  Color4 LightGoldenrodYellow = Color4{250, 250, 210, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (144, 238, 144, 255).
    /// </summary>
  Color4 LightGreen = Color4{144, 238, 144, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (211, 211, 211, 255).
    /// </summary>
  Color4 LightGray = Color4{211, 211, 211, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 182, 193, 255).
    /// </summary>
  Color4 LightPink = Color4{255, 182, 193, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 160, 122, 255).
    /// </summary>
  Color4 LightSalmon = Color4{255, 160, 122, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (32, 178, 170, 255).
    /// </summary>
  Color4 LightSeaGreen = Color4{32, 178, 170, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (135, 206, 250, 255).
    /// </summary>
  Color4 LightSkyBlue = Color4{135, 206, 250, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (119, 136, 153, 255).
    /// </summary>
  Color4 LightSlateGray = Color4{119, 136, 153, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (176, 196, 222, 255).
    /// </summary>
  Color4 LightSteelBlue = Color4{176, 196, 222, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 255, 224, 255).
    /// </summary>
  Color4 LightYellow = Color4{255, 255, 224, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 255, 0, 255).
    /// </summary>
  Color4 Lime = Color4{0, 255, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (50, 205, 50, 255).
    /// </summary>
  Color4 LimeGreen = Color4{50, 205, 50, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (250, 240, 230, 255).
    /// </summary>
  Color4 Linen = Color4{250, 240, 230, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 0, 255, 255).
    /// </summary>
  Color4 Magenta = Color4{255, 0, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (128, 0, 0, 255).
    /// </summary>
  Color4 Maroon = Color4{128, 0, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (102, 205, 170, 255).
    /// </summary>
  Color4 MediumAquamarine = Color4{102, 205, 170, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 0, 205, 255).
    /// </summary>
  Color4 MediumBlue = Color4{0, 0, 205, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (186, 85, 211, 255).
    /// </summary>
  Color4 MediumOrchid = Color4{186, 85, 211, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (147, 112, 219, 255).
    /// </summary>
  Color4 MediumPurple = Color4{147, 112, 219, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (60, 179, 113, 255).
    /// </summary>
  Color4 MediumSeaGreen = Color4{60, 179, 113, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (123, 104, 238, 255).
    /// </summary>
  Color4 MediumSlateBlue = Color4{123, 104, 238, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 250, 154, 255).
    /// </summary>
  Color4 MediumSpringGreen = Color4{0, 250, 154, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (72, 209, 204, 255).
    /// </summary>
  Color4 MediumTurquoise = Color4{72, 209, 204, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (199, 21, 133, 255).
    /// </summary>
  Color4 MediumVioletRed = Color4{199, 21, 133, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (25, 25, 112, 255).
    /// </summary>
  Color4 MidnightBlue = Color4{25, 25, 112, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (245, 255, 250, 255).
    /// </summary>
  Color4 MintCream = Color4{245, 255, 250, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 228, 225, 255).
    /// </summary>
  Color4 MistyRose = Color4{255, 228, 225, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 228, 181, 255).
    /// </summary>
  Color4 Moccasin = Color4{255, 228, 181, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 222, 173, 255).
    /// </summary>
  Color4 NavajoWhite = Color4{255, 222, 173, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 0, 128, 255).
    /// </summary>
  Color4 Navy = Color4{0, 0, 128, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (253, 245, 230, 255).
    /// </summary>
  Color4 OldLace = Color4{253, 245, 230, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (128, 128, 0, 255).
    /// </summary>
  Color4 Olive = Color4{128, 128, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (107, 142, 35, 255).
    /// </summary>
  Color4 OliveDrab = Color4{107, 142, 35, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 165, 0, 255).
    /// </summary>
  Color4 Orange = Color4{255, 165, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 69, 0, 255).
    /// </summary>
  Color4 OrangeRed = Color4{255, 69, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (218, 112, 214, 255).
    /// </summary>
  Color4 Orchid = Color4{218, 112, 214, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (238, 232, 170, 255).
    /// </summary>
  Color4 PaleGoldenrod = Color4{238, 232, 170, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (152, 251, 152, 255).
    /// </summary>
  Color4 PaleGreen = Color4{152, 251, 152, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (175, 238, 238, 255).
    /// </summary>
  Color4 PaleTurquoise = Color4{175, 238, 238, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (219, 112, 147, 255).
    /// </summary>
  Color4 PaleVioletRed = Color4{219, 112, 147, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 239, 213, 255).
    /// </summary>
  Color4 PapayaWhip = Color4{255, 239, 213, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 218, 185, 255).
    /// </summary>
  Color4 PeachPuff = Color4{255, 218, 185, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (205, 133, 63, 255).
    /// </summary>
  Color4 Peru = Color4{205, 133, 63, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 192, 203, 255).
    /// </summary>
  Color4 Pink = Color4{255, 192, 203, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (221, 160, 221, 255).
    /// </summary>
  Color4 Plum = Color4{221, 160, 221, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (176, 224, 230, 255).
    /// </summary>
  Color4 PowderBlue = Color4{176, 224, 230, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (128, 0, 128, 255).
    /// </summary>
  Color4 Purple = Color4{128, 0, 128, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 0, 0, 255).
    /// </summary>
  Color4 Red = Color4{255, 0, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (188, 143, 143, 255).
    /// </summary>
  Color4 RosyBrown = Color4{188, 143, 143, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (65, 105, 225, 255).
    /// </summary>
  Color4 RoyalBlue = Color4{65, 105, 225, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (139, 69, 19, 255).
    /// </summary>
  Color4 SaddleBrown = Color4{139, 69, 19, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (250, 128, 114, 255).
    /// </summary>
  Color4 Salmon = Color4{250, 128, 114, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (244, 164, 96, 255).
    /// </summary>
  Color4 SandyBrown = Color4{244, 164, 96, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (46, 139, 87, 255).
    /// </summary>
  Color4 SeaGreen = Color4{46, 139, 87, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 245, 238, 255).
    /// </summary>
  Color4 SeaShell = Color4{255, 245, 238, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (160, 82, 45, 255).
    /// </summary>
  Color4 Sienna = Color4{160, 82, 45, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (192, 192, 192, 255).
    /// </summary>
  Color4 Silver = Color4{192, 192, 192, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (135, 206, 235, 255).
    /// </summary>
  Color4 SkyBlue = Color4{135, 206, 235, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (106, 90, 205, 255).
    /// </summary>
  Color4 SlateBlue = Color4{106, 90, 205, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (112, 128, 144, 255).
    /// </summary>
  Color4 SlateGray = Color4{112, 128, 144, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 250, 250, 255).
    /// </summary>
  Color4 Snow = Color4{255, 250, 250, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 255, 127, 255).
    /// </summary>
  Color4 SpringGreen = Color4{0, 255, 127, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (70, 130, 180, 255).
    /// </summary>
  Color4 SteelBlue = Color4{70, 130, 180, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (210, 180, 140, 255).
    /// </summary>
  Color4 Tan = Color4{210, 180, 140, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 128, 128, 255).
    /// </summary>
  Color4 Teal = Color4{0, 128, 128, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (216, 191, 216, 255).
    /// </summary>
  Color4 Thistle = Color4{216, 191, 216, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 99, 71, 255).
    /// </summary>
  Color4 Tomato = Color4{255, 99, 71, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (64, 224, 208, 255).
    /// </summary>
  Color4 Turquoise = Color4{64, 224, 208, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (238, 130, 238, 255).
    /// </summary>
  Color4 Violet = Color4{238, 130, 238, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (245, 222, 179, 255).
    /// </summary>
  Color4 Wheat = Color4{245, 222, 179, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 255, 255, 255).
    /// </summary>
  Color4 White = Color4{255, 255, 255, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (245, 245, 245, 255).
    /// </summary>
  Color4 WhiteSmoke = Color4{245, 245, 245, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 255, 0, 255).
    /// </summary>
  Color4 Yellow = Color4{255, 255, 0, 255};

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (154, 205, 50, 255).
    /// </summary>
  Color4 YellowGreen = Color4{154, 205, 50, 255};
};
