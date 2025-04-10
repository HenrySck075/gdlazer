#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include "../../../helpers/CustomActions.hpp"
#include "../../../frameworks/graphics/containers/Container.hpp"

GDL_NS_START

class Triangles : public frameworks::Container {
private:
  ccColor3B m_triangleColor;
  int triangles = 0;
  bool j = false;

  bool init(int quantity, ccColor3B color);

  // TODO: tweak this because cocos2d space is not based on real window size
  float triangle_size = 100;
  float base_velocity = 50;

  CCSprite* makeTriangle();
  void spawnTriangle();
  // mf crashes when the node does not have a parent :laugh:
  void assignAction(CCNode* node, float startTime);
public:
  void onEnter() override;
  static Triangles* create(int quantity, ccColor3B baseColor);
};
GDL_NS_END
