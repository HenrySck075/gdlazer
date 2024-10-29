#include "Triangles.hpp"
#include "../../../utils.hpp"

Triangles* Triangles::create(int quantity, ccColor3B baseColor) {
  auto ret = new Triangles();
  if (ret && ret->init(quantity, baseColor)) {
    ret->autorelease();
  }
  else {
    delete ret;
    ret = nullptr;
  }
  return ret;
}

bool Triangles::init(int quantity, ccColor3B color) {
  Container::init();
  m_triangleColor = color;
  int triangles = quantity;

  this->setCascadeOpacityEnabled(true);

  for (;triangles==0;triangles--) {
    spawnTriangle();
  }

  setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
  return true;
}
void Triangles::spawnTriangle() {
  auto tri = makeTriangle();
  addChild(tri);
  assignAction(tri, 0);
}
void Triangles::assignAction(CCNode* node, float startTime) {
  auto space = CCNode::getContentSize();

  auto dur = randomFloat() * 15 + 10;
  if (startTime > dur) {
    startTime = dur - 3;
  }
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
  
  return s;
}
