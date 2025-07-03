#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;
// the 0-255 range one
#include <algorithm>
#include <cstdio>

struct Color4 {
  double r = 0;
  double g = 0;
  double b = 0;
  double a = 0;

  Color4 clamp() const {
#define c(v) std::max(std::min(v,1.0),0.0)
    return {c(r),c(g),c(b),c(a)};
#undef c
  }
  Color4 lighten(double amount) const {
    double scalar = std::max(255.0,255+amount)/255;
    return Color4{r*scalar,g*scalar,b*scalar,a}.clamp();
  }
  Color4 darken(double amount) const {
    double scalar = std::max(255.0,255+amount)/255;
    return Color4{r/scalar,g/scalar,b/scalar,a}.clamp();
  }
  Color4 opacity(double opacity) const {
    return Color4{r,g,b,opacity/255.f}.clamp();
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
    /*
    if (c<5) {
      r*=17;
      g*=17;
      b*=17;
    }
    */
    if (
      c==3 ||
      (c==4 && hex.c_str()[0]=='#' ) ||
      c==5 ||
      c==6
    ){
      a=255;
    }

    return {r/255.f,g/255.f,b/255.f,a/255.f};
  }

  Color4 operator+(Color4 c) const {
    return Color4(r+c.r,g+c.g,b+c.b,a+c.a).clamp();
  }
  Color4 operator-(Color4 c) const {
    return Color4(r-c.r,g-c.g,b-c.b,a-c.a).clamp();
  }

  Color4 operator/(Color4 e) {
    return {
      r/e.r,
      g/e.g,
      b/e.b,
      a/e.a
    };
  }
  Color4 operator/(double div) {
    return {
      r/div,
      g/div,
      b/div,
      a/div
    };
  }

  bool operator==(Color4 c) {
    return r==c.r && g==c.g && b==c.b && a==c.a;
  }

  // convert to cocos2d color because we're making this for cocos2d ofc
  operator ccColor3B() const {
    return ccc3(r*255,g*255,b*255);
  }
  operator ccColor4B() const {
    return ccc4(r*255,g*255,b*255,a*255);
  }
  operator ccColor4F() const {
    return ccc4f(r,g,b,a);
  }
  
  Color4(int red, int green, int blue, int alpha) : r(red/255.f), g(green/255.f), b(blue/255.f), a(alpha/255.f) {};
  Color4(double red, double green, double blue, double alpha) : r(red), g(green), b(blue), a(alpha) {};
  Color4(ccColor3B const& c) : r(c.r/255.f), g(c.g/255.f), b(c.b/255.f), a(1) {};
  Color4(ccColor4B const& c) : r(c.r/255.f), g(c.g/255.f), b(c.b/255.f), a(c.a/255.f) {};
  /// default ctor
  Color4() : r(0), g(0), b(0), a(1) {};

  // @note my addition
  // currently doing additive
  Color4 blend(Color4 const& c) {
    return Color4(
      r+(int)(c.r*(c.a/255)), 
      g+(int)(c.g*(c.a/255)), 
      b+(int)(c.b*(c.a/255)), 
      a+c.a
    ).clamp();
  }

  // uhh
  Color4 toLinear() {
    return {toLinearV(r), toLinearV(g), toLinearV(b), a};
  };

private:
  static constexpr double gamma = 2.4;
  static double toLinearV(double color) {
    if (color == 1)
        return 1;

    return color <= 0.04045f ? color / 12.92f : powf((color + 0.055f) / 1.055f, gamma);
  }
public:

  /// Gets the system color with (R, G, B, A) = (255, 255, 255, 0).
  static const Color4 Transparent;
  /// Gets the system color with (R, G, B, A) = (240, 248, 255, 255).
  static const Color4 AliceBlue;
  /// Gets the system color with (R, G, B, A) = (250, 235, 215, 255).
  static const Color4 AntiqueWhite;
  /// Gets the system color with (R, G, B, A) = (0, 255, 255, 255).
  static const Color4 Aqua;
  /// Gets the system color with (R, G, B, A) = (127, 255, 212, 255).
  static const Color4 Aquamarine;
  /// Gets the system color with (R, G, B, A) = (240, 255, 255, 255).
  static const Color4 Azure;
  /// Gets the system color with (R, G, B, A) = (245, 245, 220, 255).
  static const Color4 Beige;
  /// Gets the system color with (R, G, B, A) = (255, 228, 196, 255).
  static const Color4 Bisque;
  /// Gets the system color with (R, G, B, A) = (0, 0, 0, 255).
  static const Color4 Black;
  /// Gets the system color with (R, G, B, A) = (255, 235, 205, 255).
  static const Color4 BlanchedAlmond;
  /// Gets the system color with (R, G, B, A) = (0, 0, 255, 255).
  static const Color4 Blue;
  /// Gets the system color with (R, G, B, A) = (138, 43, 226, 255).
  static const Color4 BlueViolet;
  /// Gets the system color with (R, G, B, A) = (165, 42, 42, 255).
  static const Color4 Brown;
  /// Gets the system color with (R, G, B, A) = (222, 184, 135, 255).
  static const Color4 BurlyWood;
  /// Gets the system color with (R, G, B, A) = (95, 158, 160, 255).
  static const Color4 CadetBlue;
  /// Gets the system color with (R, G, B, A) = (127, 255, 0, 255).
  static const Color4 Chartreuse;
  /// Gets the system color with (R, G, B, A) = (210, 105, 30, 255).
  static const Color4 Chocolate;
  /// Gets the system color with (R, G, B, A) = (255, 127, 80, 255).
  static const Color4 Coral;
  /// Gets the system color with (R, G, B, A) = (100, 149, 237, 255).
  static const Color4 CornflowerBlue;
  /// Gets the system color with (R, G, B, A) = (255, 248, 220, 255).
  static const Color4 Cornsilk;
  /// Gets the system color with (R, G, B, A) = (220, 20, 60, 255).
  static const Color4 Crimson;
  /// Gets the system color with (R, G, B, A) = (0, 255, 255, 255).
  static const Color4 Cyan;
  /// Gets the system color with (R, G, B, A) = (0, 0, 139, 255).
  static const Color4 DarkBlue;
  /// Gets the system color with (R, G, B, A) = (0, 139, 139, 255).
  static const Color4 DarkCyan;
  /// Gets the system color with (R, G, B, A) = (184, 134, 11, 255).
  static const Color4 DarkGoldenrod;
  /// Gets the system color with (R, G, B, A) = (169, 169, 169, 255).
  static const Color4 DarkGray;
  /// Gets the system color with (R, G, B, A) = (0, 100, 0, 255).
  static const Color4 DarkGreen;
  /// Gets the system color with (R, G, B, A) = (189, 183, 107, 255).
  static const Color4 DarkKhaki;
  /// Gets the system color with (R, G, B, A) = (139, 0, 139, 255).
  static const Color4 DarkMagenta;
  /// Gets the system color with (R, G, B, A) = (85, 107, 47, 255).
  static const Color4 DarkOliveGreen;
  /// Gets the system color with (R, G, B, A) = (255, 140, 0, 255).
  static const Color4 DarkOrange;
  /// Gets the system color with (R, G, B, A) = (153, 50, 204, 255).
  static const Color4 DarkOrchid;
  /// Gets the system color with (R, G, B, A) = (139, 0, 0, 255).
  static const Color4 DarkRed;
  /// Gets the system color with (R, G, B, A) = (233, 150, 122, 255).
  static const Color4 DarkSalmon;
  /// Gets the system color with (R, G, B, A) = (143, 188, 139, 255).
  static const Color4 DarkSeaGreen;
  /// Gets the system color with (R, G, B, A) = (72, 61, 139, 255).
  static const Color4 DarkSlateBlue;
  /// Gets the system color with (R, G, B, A) = (47, 79, 79, 255).
  static const Color4 DarkSlateGray;
  /// Gets the system color with (R, G, B, A) = (0, 206, 209, 255).
  static const Color4 DarkTurquoise;
  /// Gets the system color with (R, G, B, A) = (148, 0, 211, 255).
  static const Color4 DarkViolet;
  /// Gets the system color with (R, G, B, A) = (255, 20, 147, 255).
  static const Color4 DeepPink;
  /// Gets the system color with (R, G, B, A) = (0, 191, 255, 255).
  static const Color4 DeepSkyBlue;
  /// Gets the system color with (R, G, B, A) = (105, 105, 105, 255).
  static const Color4 DimGray;
  /// Gets the system color with (R, G, B, A) = (30, 144, 255, 255).
  static const Color4 DodgerBlue;
  /// Gets the system color with (R, G, B, A) = (178, 34, 34, 255).
  static const Color4 Firebrick;
  /// Gets the system color with (R, G, B, A) = (255, 250, 240, 255).
  static const Color4 FloralWhite;
  /// Gets the system color with (R, G, B, A) = (34, 139, 34, 255).
  static const Color4 ForestGreen;
  /// Gets the system color with (R, G, B, A) = (255, 0, 255, 255).
  static const Color4 Fuchsia;
  /// Gets the system color with (R, G, B, A) = (220, 220, 220, 255).
  static const Color4 Gainsboro;
  /// Gets the system color with (R, G, B, A) = (248, 248, 255, 255).
  static const Color4 GhostWhite;
  /// Gets the system color with (R, G, B, A) = (255, 215, 0, 255).
  static const Color4 Gold;
  /// Gets the system color with (R, G, B, A) = (218, 165, 32, 255).
  static const Color4 Goldenrod;
  /// Gets the system color with (R, G, B, A) = (128, 128, 128, 255).
  static const Color4 Gray;
  /// Gets the system color with (R, G, B, A) = (0, 128, 0, 255).
  static const Color4 Green;
  /// Gets the system color with (R, G, B, A) = (173, 255, 47, 255).
  static const Color4 GreenYellow;
  /// Gets the system color with (R, G, B, A) = (240, 255, 240, 255).
  static const Color4 Honeydew;
  /// Gets the system color with (R, G, B, A) = (255, 105, 180, 255).
  static const Color4 HotPink;
  /// Gets the system color with (R, G, B, A) = (205, 92, 92, 255).
  static const Color4 IndianRed;
  /// Gets the system color with (R, G, B, A) = (75, 0, 130, 255).
  static const Color4 Indigo;
  /// Gets the system color with (R, G, B, A) = (255, 255, 240, 255).
  static const Color4 Ivory;
  /// Gets the system color with (R, G, B, A) = (240, 230, 140, 255).
  static const Color4 Khaki;
  /// Gets the system color with (R, G, B, A) = (230, 230, 250, 255).
  static const Color4 Lavender;
  /// Gets the system color with (R, G, B, A) = (255, 240, 245, 255).
  static const Color4 LavenderBlush;
  /// Gets the system color with (R, G, B, A) = (124, 252, 0, 255).
  static const Color4 LawnGreen;
  /// Gets the system color with (R, G, B, A) = (255, 250, 205, 255).
  static const Color4 LemonChiffon;
  /// Gets the system color with (R, G, B, A) = (173, 216, 230, 255).
  static const Color4 LightBlue;
  /// Gets the system color with (R, G, B, A) = (240, 128, 128, 255).
  static const Color4 LightCoral;
  /// Gets the system color with (R, G, B, A) = (224, 255, 255, 255).
  static const Color4 LightCyan;
  /// Gets the system color with (R, G, B, A) = (250, 250, 210, 255).
  static const Color4 LightGoldenrodYellow;
  /// Gets the system color with (R, G, B, A) = (144, 238, 144, 255).
  static const Color4 LightGreen;
  /// Gets the system color with (R, G, B, A) = (211, 211, 211, 255).
  static const Color4 LightGray;
  /// Gets the system color with (R, G, B, A) = (255, 182, 193, 255).
  static const Color4 LightPink;
  /// Gets the system color with (R, G, B, A) = (255, 160, 122, 255).
  static const Color4 LightSalmon;
  /// Gets the system color with (R, G, B, A) = (32, 178, 170, 255).
  static const Color4 LightSeaGreen;
  /// Gets the system color with (R, G, B, A) = (135, 206, 250, 255).
  static const Color4 LightSkyBlue;
  /// Gets the system color with (R, G, B, A) = (119, 136, 153, 255).
  static const Color4 LightSlateGray;
  /// Gets the system color with (R, G, B, A) = (176, 196, 222, 255).
  static const Color4 LightSteelBlue;
  /// Gets the system color with (R, G, B, A) = (255, 255, 224, 255).
  static const Color4 LightYellow;
  /// Gets the system color with (R, G, B, A) = (0, 255, 0, 255).
  static const Color4 Lime;
  /// Gets the system color with (R, G, B, A) = (50, 205, 50, 255).
  static const Color4 LimeGreen;
  /// Gets the system color with (R, G, B, A) = (250, 240, 230, 255).
  static const Color4 Linen;
  /// Gets the system color with (R, G, B, A) = (255, 0, 255, 255).
  static const Color4 Magenta;
  /// Gets the system color with (R, G, B, A) = (128, 0, 0, 255).
  static const Color4 Maroon;
  /// Gets the system color with (R, G, B, A) = (102, 205, 170, 255).
  static const Color4 MediumAquamarine;
  /// Gets the system color with (R, G, B, A) = (0, 0, 205, 255).
  static const Color4 MediumBlue;
  /// Gets the system color with (R, G, B, A) = (186, 85, 211, 255).
  static const Color4 MediumOrchid;
  /// Gets the system color with (R, G, B, A) = (147, 112, 219, 255).
  static const Color4 MediumPurple;
  /// Gets the system color with (R, G, B, A) = (60, 179, 113, 255).
  static const Color4 MediumSeaGreen;
  /// Gets the system color with (R, G, B, A) = (123, 104, 238, 255).
  static const Color4 MediumSlateBlue;
  /// Gets the system color with (R, G, B, A) = (0, 250, 154, 255).
  static const Color4 MediumSpringGreen;
  /// Gets the system color with (R, G, B, A) = (72, 209, 204, 255).
  static const Color4 MediumTurquoise;
  /// Gets the system color with (R, G, B, A) = (199, 21, 133, 255).
  static const Color4 MediumVioletRed;
  /// Gets the system color with (R, G, B, A) = (25, 25, 112, 255).
  static const Color4 MidnightBlue;
  /// Gets the system color with (R, G, B, A) = (245, 255, 250, 255).
  static const Color4 MintCream;
  /// Gets the system color with (R, G, B, A) = (255, 228, 225, 255).
  static const Color4 MistyRose;
  /// Gets the system color with (R, G, B, A) = (255, 228, 181, 255).
  static const Color4 Moccasin;
  /// Gets the system color with (R, G, B, A) = (255, 222, 173, 255).
  static const Color4 NavajoWhite;
  /// Gets the system color with (R, G, B, A) = (0, 0, 128, 255).
  static const Color4 Navy;
  /// Gets the system color with (R, G, B, A) = (253, 245, 230, 255).
  static const Color4 OldLace;
  /// Gets the system color with (R, G, B, A) = (128, 128, 0, 255).
  static const Color4 Olive;
  /// Gets the system color with (R, G, B, A) = (107, 142, 35, 255).
  static const Color4 OliveDrab;
  /// Gets the system color with (R, G, B, A) = (255, 165, 0, 255).
  static const Color4 Orange;
  /// Gets the system color with (R, G, B, A) = (255, 69, 0, 255).
  static const Color4 OrangeRed;
  /// Gets the system color with (R, G, B, A) = (218, 112, 214, 255).
  static const Color4 Orchid;
  /// Gets the system color with (R, G, B, A) = (238, 232, 170, 255).
  static const Color4 PaleGoldenrod;
  /// Gets the system color with (R, G, B, A) = (152, 251, 152, 255).
  static const Color4 PaleGreen;
  /// Gets the system color with (R, G, B, A) = (175, 238, 238, 255).
  static const Color4 PaleTurquoise;
  /// Gets the system color with (R, G, B, A) = (219, 112, 147, 255).
  static const Color4 PaleVioletRed;
  /// Gets the system color with (R, G, B, A) = (255, 239, 213, 255).
  static const Color4 PapayaWhip;
  /// Gets the system color with (R, G, B, A) = (255, 218, 185, 255).
  static const Color4 PeachPuff;
  /// Gets the system color with (R, G, B, A) = (205, 133, 63, 255).
  static const Color4 Peru;
  /// Gets the system color with (R, G, B, A) = (255, 192, 203, 255).
  static const Color4 Pink;
  /// Gets the system color with (R, G, B, A) = (221, 160, 221, 255).
  static const Color4 Plum;
  /// Gets the system color with (R, G, B, A) = (176, 224, 230, 255).
  static const Color4 PowderBlue;
  /// Gets the system color with (R, G, B, A) = (128, 0, 128, 255).
  static const Color4 Purple;
  /// Gets the system color with (R, G, B, A) = (255, 0, 0, 255).
  static const Color4 Red;
  /// Gets the system color with (R, G, B, A) = (188, 143, 143, 255).
  static const Color4 RosyBrown;
  /// Gets the system color with (R, G, B, A) = (65, 105, 225, 255).
  static const Color4 RoyalBlue;
  /// Gets the system color with (R, G, B, A) = (139, 69, 19, 255).
  static const Color4 SaddleBrown;
  /// Gets the system color with (R, G, B, A) = (250, 128, 114, 255).
  static const Color4 Salmon;
  /// Gets the system color with (R, G, B, A) = (244, 164, 96, 255).
  static const Color4 SandyBrown;
  /// Gets the system color with (R, G, B, A) = (46, 139, 87, 255).
  static const Color4 SeaGreen;
  /// Gets the system color with (R, G, B, A) = (255, 245, 238, 255).
  static const Color4 SeaShell;
  /// Gets the system color with (R, G, B, A) = (160, 82, 45, 255).
  static const Color4 Sienna;
  /// Gets the system color with (R, G, B, A) = (192, 192, 192, 255).
  static const Color4 Silver;
  /// Gets the system color with (R, G, B, A) = (135, 206, 235, 255).
  static const Color4 SkyBlue;
  /// Gets the system color with (R, G, B, A) = (106, 90, 205, 255).
  static const Color4 SlateBlue;
  /// Gets the system color with (R, G, B, A) = (112, 128, 144, 255).
  static const Color4 SlateGray;
  /// Gets the system color with (R, G, B, A) = (255, 250, 250, 255).
  static const Color4 Snow;
  /// Gets the system color with (R, G, B, A) = (0, 255, 127, 255).
  static const Color4 SpringGreen;
  /// Gets the system color with (R, G, B, A) = (70, 130, 180, 255).
  static const Color4 SteelBlue;
  /// Gets the system color with (R, G, B, A) = (210, 180, 140, 255).
  static const Color4 Tan;
  /// Gets the system color with (R, G, B, A) = (0, 128, 128, 255).
  static const Color4 Teal;
  /// Gets the system color with (R, G, B, A) = (216, 191, 216, 255).
  static const Color4 Thistle;
  /// Gets the system color with (R, G, B, A) = (255, 99, 71, 255).
  static const Color4 Tomato;
  /// Gets the system color with (R, G, B, A) = (64, 224, 208, 255).
  static const Color4 Turquoise;
  /// Gets the system color with (R, G, B, A) = (238, 130, 238, 255).
  static const Color4 Violet;
  /// Gets the system color with (R, G, B, A) = (245, 222, 179, 255).
  static const Color4 Wheat;
  /// Gets the system color with (R, G, B, A) = (255, 255, 255, 255).
  static const Color4 White;
  /// Gets the system color with (R, G, B, A) = (245, 245, 245, 255).
  static const Color4 WhiteSmoke;
  /// Gets the system color with (R, G, B, A) = (255, 255, 0, 255).
  static const Color4 Yellow;
  /// Gets the system color with (R, G, B, A) = (154, 205, 50, 255).
  static const Color4 YellowGreen;
};
