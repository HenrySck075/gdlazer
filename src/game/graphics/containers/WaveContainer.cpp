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
  auto left = ccp(0,h);
  auto right = ccp(node->getContentWidth(),h);

  // i still have naming skill issues
  CCPoint last;
  if (rotateLeft) {
    last = right.rotateByAngle(left, degreeToRadius(angle));
    float remainingDistRatio = last.x/right.x;
    last = ccp(right.x,last.y*remainingDistRatio);
  } else {
    last = left.rotateByAngle(right, -degreeToRadius(angle));
    float remainingDistRatio = last.x/right.x;
    last = ccp(left.x,last.y);
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
    wave->setAnchorPoint(ccp(0.5,1)); 
    wave->setPosition(ccp(w,-offset-2));
    return wave;
}

WaveContainer* WaveContainer::create(ColorScheme color, CCNode* body) {
    create_class(WaveContainer, init, color, body);
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
    this->setContentSize(s);
    auto w = s.width/2;

    this->wave1 = createWave(w,k, angle1, provider->Light4());
    this->wave2 = createWave(w,k, angle2, provider->Light3());
    this->wave3 = createWave(w,k, angle3, provider->Dark4());
    this->wave4 = createWave(w,k, angle4, provider->Dark3());

    this->addChild(wave1);
    this->addChild(wave2);
    this->addChild(wave3);
    this->addChild(wave4);

    this->body = pBody; // mb
    body->setAnchorPoint(ccp(0.5,1));
    body->setPosition(ccp(s.width/2, 0));
    this->addChild(body);
    body->setContentSize(k);

    return true;
}


void WaveContainer::onOpen() {
    hiding = false;
    auto opacity = this->getOpacity();
    this->setOpacity(0);
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
    auto h = this->getContentHeight();
    
#define j(id, dist) \
    pos##id = wave##id->getPositionY(); \
    wave##id->runAction(CCEaseSineOut::create(CCMoveTo::create(appearDuration, ccp(wave##id->getPositionX(),h+(dist/1366*h)))))
    j(1,930.f);
    j(2,560.f);
    j(3,390.f);
    j(4,220.f);
    body->runAction(CCEaseSineOut::create(CCMoveTo::create(appearDuration, ccp(body->getPositionX(),h))));
    this->runAction(CCFadeTo::create(0.1f, opacity));

    FMODAudioEngine::sharedEngine()->playEffect("wave-pop-in.wav"_spr);
#undef j
}

void WaveContainer::onClose() {
    // nuh uh
    if (hiding) return;
    this->stopAllActions();

#define j(id) wave##id->runAction(CCEaseSineIn::create(CCMoveTo::create(disappearDuration, ccp(wave##id->getPositionX(),pos##id))))
    j(1);
    j(2);
    j(3);
    j(4);
    body->runAction(CCEaseSineIn::create(CCMoveTo::create(disappearDuration, ccp(body->getPositionX(),0))));
    this->runAction(CCSequence::create( 
          CCFadeTo::create(0.1f,0),
          CCDelayTime::create(disappearDuration-0.1f),
          CCRemoveSelf::create(),
          nullptr
    ));
    this->registerWithTouchDispatcher();

    FMODAudioEngine::sharedEngine()->playEffect("overlay-big-pop-out.wav"_spr);
    hiding = true;
#undef j
}

void WaveContainer::onClick() {
    if (!touchBoundary.containsPoint(touchLoc) && !touchBoundary.containsPoint(t->getLocation())) hide();
}

