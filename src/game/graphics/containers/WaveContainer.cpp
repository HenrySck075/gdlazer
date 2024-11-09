#include "WaveContainer.hpp"
#include "../../../utils.hpp"
#include "ccTypes.h"
#include <cstdlib>
#include <tuple>

std::tuple<CCDrawNode*, float> drawWave(CCSize size, ccColor4B color, float angle) {
  /*
  auto ren = CCRenderTexture::create(size.width, size.height);
  ren->begin();
  */
  CCDrawNode* node = CCDrawNode::create();
  node->setContentSize(size);
  bool rotateLeft = angle<0;
  angle = abs(angle);
  auto h = node->getContentHeight();
  CCPoint left = {0,h};
  auto right = ccp(node->getContentWidth(),h);

  // i still have naming skill issues
  CCPoint last = {0,0};
  if (rotateLeft) {
    last = right.rotateByAngle(left, kmDegreesToRadians(angle));
    float remainingDistRatio = last.x/right.x;
    last = CCPoint{right.x,last.y*remainingDistRatio};
  } else {
    last = left.rotateByAngle(right, -kmDegreesToRadians(angle));
    float remainingDistRatio = last.x/right.x;
    last = CCPoint{left.x,last.y};
  }
  CCPoint j[3] = {left, right, last};
  auto color4f = ccc4FFromccc4B(color);
  node->drawPolygon(j, 3, color4f,0,color4f);

  // debug
  /*
  node->drawDot(left, 3, ccc4f(1, 0, 0, 1));
  node->drawDot(right, 3, ccc4f(0, 1, 0, 1));
  node->drawDot(last, 3, ccc4f(0, 0, 1, 1));
  */
  node->drawRect(ccp(0,-(h/2)), size, color4f,0,color4f);

  return std::make_tuple(node, last.y-h);
  /*
  node->visit();
  ren->end();
  ren->retain();
  return ren;
  */
}

CCDrawNode* WaveContainer::createWave(float w, CCSize size, float angle, ccColor4B col) {
  CCDrawNode* wave;
  float offset;
  std::tie(wave, offset) = drawWave(size, col, angle); 
  wave->setAnchorPoint({0.5,1}); 
  wave->setPosition({w,-offset-2});
  return wave;
}

WaveContainer* WaveContainer::create(ColorScheme color, CCNode* body) {
  $create_class(WaveContainer, init, color, body);
}

bool WaveContainer::init(ColorScheme color, CCNode* pBody) {
  provider = OverlayColorProvider::create(color);
  provider->retain();
  return customSetup(pBody);
}

bool WaveContainer::customSetup(CCNode* pBody) {
  auto s = CCDirector::sharedDirector()->getWinSize();
  auto k = CCSize{s.width*0.8f, s.height};
  touchBoundary = CCRect((s.width-k.width)/2,0,k.width,k.height);
  setContentSize(s);
  auto w = s.width/2;

  wave1 = createWave(w,k, angle1, provider->Light4());
  wave2 = createWave(w,k, angle2, provider->Light3());
  wave3 = createWave(w,k, angle3, provider->Dark4());
  wave4 = createWave(w,k, angle4, provider->Dark3());

  addChild(wave1);
  addChild(wave2);
  addChild(wave3);
  addChild(wave4);

  body = pBody; // mb
  body->setAnchorPoint({0.5,1});
  body->setPosition({s.width/2, 0});
  addChild(body);
  body->setContentSize(k);

  return true;
}


void WaveContainer::onOpen() {
  hiding = false;
  auto opacity = getOpacity();
  setOpacity(0);
  CCDirector::sharedDirector()->getRunningScene()->addChild(this);
  auto h = getContentHeight();
  
#define j(id, dist) \
  pos##id = wave##id->getPositionY(); \
  wave##id->runAction(CCEaseSineOut::create(CCMoveTo::create(appearDuration, ccp(wave##id->getPositionX(),h+(dist/1366*h)))))
  j(1,930.f);
  j(2,560.f);
  j(3,390.f);
  j(4,220.f);
  body->runAction(CCEaseSineOut::create(CCMoveTo::create(appearDuration, ccp(body->getPositionX(),h))));
  runAction(CCFadeTo::create(0.1f, opacity));

  FMODAudioEngine::sharedEngine()->playEffect("wave-pop-in.wav"_spr);
#undef j
}

void WaveContainer::onClose() {
  // nuh uh
  if (hiding) return;
  stopAllActions();

#define j(id) wave##id->runAction(CCEaseSineIn::create(CCMoveTo::create(disappearDuration, ccp(wave##id->getPositionX(),pos##id))))
  j(1);
  j(2);
  j(3);
  j(4);
  body->runAction(CCEaseSineIn::create(CCMoveTo::create(disappearDuration, ccp(body->getPositionX(),0))));
  runAction(CCSequence::create( 
      CCFadeTo::create(0.1f,0),
      CCDelayTime::create(disappearDuration-0.1f),
      CCRemoveSelf::create(),
      nullptr
  ));
  registerWithTouchDispatcher();

  FMODAudioEngine::sharedEngine()->playEffect("overlay-big-pop-out.wav"_spr);
  hiding = true;
#undef j
}
void WaveContainer::onMouseDown(MouseEvent* e) {
  touchLoc = e->position;
}
void WaveContainer::onClick(MouseEvent* e) {
  if (!touchBoundary.containsPoint(touchLoc) && !touchBoundary.containsPoint(e->position)) hide();
}
