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
#define c(v) std::max(std::min(v,255),0)
    return {c(r),c(g),c(b),c(a)};
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

  Color4 operator+(Color4 c) {
    return Color4(r+c.r,g+c.g,b+c.b,a+c.a).clamp();
  }


  // convert to cocos2d color because we're making this for cocos2d ofc
  operator ccColor3B() const {
    return ccc3(r,g,b);
  }
  operator ccColor4B() const {
    return ccc4(r,g,b,a);
  }
  operator ccColor4F() const {
    return ccc4f(r/255,g/255,b/255,a/255);
  }

  Color4(int red, int green, int blue, int alpha) : r(red), g(green), b(blue), a(alpha) {};
  Color4(ccColor3B c) : r(c.r), g(c.g), b(c.b), a(255) {};
  Color4(ccColor4B c) : r(c.r), g(c.g), b(c.b), a(c.a) {};

  // @note my addition
  // currently doing additive
  Color4 blend(Color4 c) {
    return Color4(
      r+(int)(c.r*(c.a/255)), 
      g+(int)(c.g*(c.a/255)), 
      b+(int)(c.b*(c.a/255)), 
      a+c.a
    ).clamp();
  }
};
struct Color4Defined {
    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 255, 255, 0).
    /// </summary>
  static Color4 Transparent;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (240, 248, 255, 255).
    /// </summary>
  static Color4 AliceBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (250, 235, 215, 255).
    /// </summary>
  static Color4 AntiqueWhite;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 255, 255, 255).
    /// </summary>
  static Color4 Aqua;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (127, 255, 212, 255).
    /// </summary>
  static Color4 Aquamarine;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (240, 255, 255, 255).
    /// </summary>
  static Color4 Azure;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (245, 245, 220, 255).
    /// </summary>
  static Color4 Beige;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 228, 196, 255).
    /// </summary>
  static Color4 Bisque;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 0, 0, 255).
    /// </summary>
  static Color4 Black;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 235, 205, 255).
    /// </summary>
  static Color4 BlanchedAlmond;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 0, 255, 255).
    /// </summary>
  static Color4 Blue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (138, 43, 226, 255).
    /// </summary>
  static Color4 BlueViolet;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (165, 42, 42, 255).
    /// </summary>
  static Color4 Brown;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (222, 184, 135, 255).
    /// </summary>
  static Color4 BurlyWood;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (95, 158, 160, 255).
    /// </summary>
  static Color4 CadetBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (127, 255, 0, 255).
    /// </summary>
  static Color4 Chartreuse;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (210, 105, 30, 255).
    /// </summary>
  static Color4 Chocolate;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 127, 80, 255).
    /// </summary>
  static Color4 Coral;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (100, 149, 237, 255).
    /// </summary>
  static Color4 CornflowerBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 248, 220, 255).
    /// </summary>
  static Color4 Cornsilk;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (220, 20, 60, 255).
    /// </summary>
  static Color4 Crimson;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 255, 255, 255).
    /// </summary>
  static Color4 Cyan;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 0, 139, 255).
    /// </summary>
  static Color4 DarkBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 139, 139, 255).
    /// </summary>
  static Color4 DarkCyan;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (184, 134, 11, 255).
    /// </summary>
  static Color4 DarkGoldenrod;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (169, 169, 169, 255).
    /// </summary>
  static Color4 DarkGray;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 100, 0, 255).
    /// </summary>
  static Color4 DarkGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (189, 183, 107, 255).
    /// </summary>
  static Color4 DarkKhaki;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (139, 0, 139, 255).
    /// </summary>
  static Color4 DarkMagenta;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (85, 107, 47, 255).
    /// </summary>
  static Color4 DarkOliveGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 140, 0, 255).
    /// </summary>
  static Color4 DarkOrange;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (153, 50, 204, 255).
    /// </summary>
  static Color4 DarkOrchid;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (139, 0, 0, 255).
    /// </summary>
  static Color4 DarkRed;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (233, 150, 122, 255).
    /// </summary>
  static Color4 DarkSalmon;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (143, 188, 139, 255).
    /// </summary>
  static Color4 DarkSeaGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (72, 61, 139, 255).
    /// </summary>
  static Color4 DarkSlateBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (47, 79, 79, 255).
    /// </summary>
  static Color4 DarkSlateGray;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 206, 209, 255).
    /// </summary>
  static Color4 DarkTurquoise;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (148, 0, 211, 255).
    /// </summary>
  static Color4 DarkViolet;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 20, 147, 255).
    /// </summary>
  static Color4 DeepPink;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 191, 255, 255).
    /// </summary>
  static Color4 DeepSkyBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (105, 105, 105, 255).
    /// </summary>
  static Color4 DimGray;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (30, 144, 255, 255).
    /// </summary>
  static Color4 DodgerBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (178, 34, 34, 255).
    /// </summary>
  static Color4 Firebrick;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 250, 240, 255).
    /// </summary>
  static Color4 FloralWhite;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (34, 139, 34, 255).
    /// </summary>
  static Color4 ForestGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 0, 255, 255).
    /// </summary>
  static Color4 Fuchsia;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (220, 220, 220, 255).
    /// </summary>
  static Color4 Gainsboro;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (248, 248, 255, 255).
    /// </summary>
  static Color4 GhostWhite;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 215, 0, 255).
    /// </summary>
  static Color4 Gold;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (218, 165, 32, 255).
    /// </summary>
  static Color4 Goldenrod;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (128, 128, 128, 255).
    /// </summary>
  static Color4 Gray;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 128, 0, 255).
    /// </summary>
  static Color4 Green;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (173, 255, 47, 255).
    /// </summary>
  static Color4 GreenYellow;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (240, 255, 240, 255).
    /// </summary>
  static Color4 Honeydew;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 105, 180, 255).
    /// </summary>
  static Color4 HotPink;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (205, 92, 92, 255).
    /// </summary>
  static Color4 IndianRed;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (75, 0, 130, 255).
    /// </summary>
  static Color4 Indigo;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 255, 240, 255).
    /// </summary>
  static Color4 Ivory;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (240, 230, 140, 255).
    /// </summary>
  static Color4 Khaki;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (230, 230, 250, 255).
    /// </summary>
  static Color4 Lavender;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 240, 245, 255).
    /// </summary>
  static Color4 LavenderBlush;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (124, 252, 0, 255).
    /// </summary>
  static Color4 LawnGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 250, 205, 255).
    /// </summary>
  static Color4 LemonChiffon;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (173, 216, 230, 255).
    /// </summary>
  static Color4 LightBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (240, 128, 128, 255).
    /// </summary>
  static Color4 LightCoral;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (224, 255, 255, 255).
    /// </summary>
  static Color4 LightCyan;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (250, 250, 210, 255).
    /// </summary>
  static Color4 LightGoldenrodYellow;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (144, 238, 144, 255).
    /// </summary>
  static Color4 LightGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (211, 211, 211, 255).
    /// </summary>
  static Color4 LightGray;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 182, 193, 255).
    /// </summary>
  static Color4 LightPink;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 160, 122, 255).
    /// </summary>
  static Color4 LightSalmon;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (32, 178, 170, 255).
    /// </summary>
  static Color4 LightSeaGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (135, 206, 250, 255).
    /// </summary>
  static Color4 LightSkyBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (119, 136, 153, 255).
    /// </summary>
  static Color4 LightSlateGray;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (176, 196, 222, 255).
    /// </summary>
  static Color4 LightSteelBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 255, 224, 255).
    /// </summary>
  static Color4 LightYellow;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 255, 0, 255).
    /// </summary>
  static Color4 Lime;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (50, 205, 50, 255).
    /// </summary>
  static Color4 LimeGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (250, 240, 230, 255).
    /// </summary>
  static Color4 Linen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 0, 255, 255).
    /// </summary>
  static Color4 Magenta;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (128, 0, 0, 255).
    /// </summary>
  static Color4 Maroon;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (102, 205, 170, 255).
    /// </summary>
  static Color4 MediumAquamarine;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 0, 205, 255).
    /// </summary>
  static Color4 MediumBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (186, 85, 211, 255).
    /// </summary>
  static Color4 MediumOrchid;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (147, 112, 219, 255).
    /// </summary>
  static Color4 MediumPurple;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (60, 179, 113, 255).
    /// </summary>
  static Color4 MediumSeaGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (123, 104, 238, 255).
    /// </summary>
  static Color4 MediumSlateBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 250, 154, 255).
    /// </summary>
  static Color4 MediumSpringGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (72, 209, 204, 255).
    /// </summary>
  static Color4 MediumTurquoise;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (199, 21, 133, 255).
    /// </summary>
  static Color4 MediumVioletRed;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (25, 25, 112, 255).
    /// </summary>
  static Color4 MidnightBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (245, 255, 250, 255).
    /// </summary>
  static Color4 MintCream;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 228, 225, 255).
    /// </summary>
  static Color4 MistyRose;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 228, 181, 255).
    /// </summary>
  static Color4 Moccasin;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 222, 173, 255).
    /// </summary>
  static Color4 NavajoWhite;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 0, 128, 255).
    /// </summary>
  static Color4 Navy;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (253, 245, 230, 255).
    /// </summary>
  static Color4 OldLace;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (128, 128, 0, 255).
    /// </summary>
  static Color4 Olive;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (107, 142, 35, 255).
    /// </summary>
  static Color4 OliveDrab;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 165, 0, 255).
    /// </summary>
  static Color4 Orange;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 69, 0, 255).
    /// </summary>
  static Color4 OrangeRed;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (218, 112, 214, 255).
    /// </summary>
  static Color4 Orchid;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (238, 232, 170, 255).
    /// </summary>
  static Color4 PaleGoldenrod;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (152, 251, 152, 255).
    /// </summary>
  static Color4 PaleGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (175, 238, 238, 255).
    /// </summary>
  static Color4 PaleTurquoise;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (219, 112, 147, 255).
    /// </summary>
  static Color4 PaleVioletRed;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 239, 213, 255).
    /// </summary>
  static Color4 PapayaWhip;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 218, 185, 255).
    /// </summary>
  static Color4 PeachPuff;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (205, 133, 63, 255).
    /// </summary>
  static Color4 Peru;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 192, 203, 255).
    /// </summary>
  static Color4 Pink;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (221, 160, 221, 255).
    /// </summary>
  static Color4 Plum;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (176, 224, 230, 255).
    /// </summary>
  static Color4 PowderBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (128, 0, 128, 255).
    /// </summary>
  static Color4 Purple;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 0, 0, 255).
    /// </summary>
  static Color4 Red;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (188, 143, 143, 255).
    /// </summary>
  static Color4 RosyBrown;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (65, 105, 225, 255).
    /// </summary>
  static Color4 RoyalBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (139, 69, 19, 255).
    /// </summary>
  static Color4 SaddleBrown;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (250, 128, 114, 255).
    /// </summary>
  static Color4 Salmon;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (244, 164, 96, 255).
    /// </summary>
  static Color4 SandyBrown;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (46, 139, 87, 255).
    /// </summary>
  static Color4 SeaGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 245, 238, 255).
    /// </summary>
  static Color4 SeaShell;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (160, 82, 45, 255).
    /// </summary>
  static Color4 Sienna;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (192, 192, 192, 255).
    /// </summary>
  static Color4 Silver;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (135, 206, 235, 255).
    /// </summary>
  static Color4 SkyBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (106, 90, 205, 255).
    /// </summary>
  static Color4 SlateBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (112, 128, 144, 255).
    /// </summary>
  static Color4 SlateGray;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 250, 250, 255).
    /// </summary>
  static Color4 Snow;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 255, 127, 255).
    /// </summary>
  static Color4 SpringGreen;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (70, 130, 180, 255).
    /// </summary>
  static Color4 SteelBlue;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (210, 180, 140, 255).
    /// </summary>
  static Color4 Tan;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (0, 128, 128, 255).
    /// </summary>
  static Color4 Teal;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (216, 191, 216, 255).
    /// </summary>
  static Color4 Thistle;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 99, 71, 255).
    /// </summary>
  static Color4 Tomato;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (64, 224, 208, 255).
    /// </summary>
  static Color4 Turquoise;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (238, 130, 238, 255).
    /// </summary>
  static Color4 Violet;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (245, 222, 179, 255).
    /// </summary>
  static Color4 Wheat;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 255, 255, 255).
    /// </summary>
  static Color4 White;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (245, 245, 245, 255).
    /// </summary>
  static Color4 WhiteSmoke;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (255, 255, 0, 255).
    /// </summary>
  static Color4 Yellow;

    /// <summary>
    /// Gets the system color with (R, G, B, A) = (154, 205, 50, 255).
    /// </summary>
  static Color4 YellowGreen;
};
