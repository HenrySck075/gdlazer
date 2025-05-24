#include "Triangles.hpp"
#include "../../../utils.hpp"

GDL_NS_START
Triangles* Triangles::create(int quantity, ccColor3B baseColor) {
  $create_class(Triangles, init, quantity, baseColor);
}

bool Triangles::init(int quantity, ccColor3B color) {
  if (!Container::init()) return false;
  m_triangleColor = color;
  triangles = quantity;
  setContentSize({100,100},frameworks::Unit::Percent);

  setCascadeOpacityEnabled(true);

  return true;
}
void Triangles::spawnTriangle() {
  auto tri = makeTriangle();
  addChild(tri);
  assignAction(tri, 0);
}
void Triangles::spawnTriangleSkipped() {
  auto tri = makeTriangle();
  addChild(tri);
  assignAction(tri, randomFloat()/2+0.25);
}
void Triangles::assignAction(CCNode* node, float startTimeDelta) {
  auto space = CCNode::getContentSize();

  auto dur = randomFloat() * 15 + 10;
  float startTime = dur * startTimeDelta;
  float offset = randomFloat()*2;
  auto moveToAction = CCMoveTo::create(
    dur, 
    { 
      node->getPositionX() + offset*randomBool(), 
      space.height + node->getContentHeight()*node->getScale()
    }
  );

  node->runAction(CCSequence::create( 
    CCActionSkip::create(startTime, moveToAction), 
    CCCallFunc::create(this, callfunc_selector(Triangles::spawnTriangle)), 
    CCRemoveSelf::create(), 
    nullptr 
  ));
 

}
CCSprite* Triangles::makeTriangle() {
  auto s = CCSprite::createWithSpriteFrameName("tri_fill.png"_spr);
  int shiftValue = randomFloat()*20;
  ccColor3B triangleColor = m_triangleColor;
  triangleColor.r += shiftValue;
  triangleColor.g += shiftValue;
  triangleColor.b += shiftValue;
  s->setColor(triangleColor);
  
  auto size = randomFloat() * 2+0.5;
  if (size == 0) { size = 1; }
  s->setScale(size);

  auto space = CCNode::getContentSize();

  s->setPosition({ randomFloat() * space.width, 0});
  s->setAnchorPoint({ 0.5, 1 });

  s->setOpacity(getDisplayedOpacity());
  
  return s;
}


void Triangles::onEnter() {
  CCNode::onEnter();
  queueInMainThread([this]{
    if (!j) {
      for (;triangles!=0;triangles--) {
        spawnTriangleSkipped();
      }
    }
    j = true;
  });
}

GDL_NS_END
