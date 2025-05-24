#pragma once
#include <Geode/Geode.hpp>
#include "../../../../frameworks/screens/Screen.hpp"
#include "../../../graphics/ui/OsuText.hpp"

using namespace geode::prelude;

GDL_NS_START
class IntroTriangles : public frameworks::Screen {
private:
  geode::Ref<OsuText> m_welcomeText;
  geode::Ref<CCLayer> m_textGlitch;
  geode::Ref<CCLayer> m_iconSets;

  float text_1 = 0.2;
  float text_2 = 0.4;
  float text_3 = 0.7;
  float text_4 = 0.9;
  float text_glitch = 1.06;

  float rulesets_1 = 1.45;
  float rulesets_2 = 1.65;
  float rulesets_3 = 1.85;

  float logo_scale_duration = 0.92;
  float logo_1 = 2.08;
  float logo_2 = logo_1 + logo_scale_duration;

  float time_between_triangles = 0.022;

  // stage 1: welcome to osu!
  void text_1_func();
  void text_2_func();
  void text_3_func();
  void text_4_func();
  void text_4_set_spacing(float per);

  void drawTriangle(CCDrawNode* drawNode, CCPoint& point1, CCPoint& point2, CCPoint& point3, const ccColor4F& fill) {
    CCPoint m[3] = {point1 , point2, point3};
    drawNode->drawPolygon(m, 3, fill,0.5, ccColor4F{1,1,1,255});
  }
  void renderTriangles();

  // stage 2: iconsets
  void rulesets_1_func();
  void rulesets_1_change_gap(float gap);
  void rulesets_2_func();
  void rulesets_3_func();

  // stage 3: osu! logo
  void logo_1_func();
  void logo_scale();

public:
  bool init();
  static IntroTriangles* create();

  void onExiting(frameworks::ScreenTransitionEvent e);
};

GDL_NS_END