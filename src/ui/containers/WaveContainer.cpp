#include "WaveContainer.hpp"
#include "../../utils.hpp"
#include "ccTypes.h"
#include <cstdlib>

CCRenderTexture* drawWave(CCSize size, ccColor3B color, float angle) {
  auto ren = CCRenderTexture::create(size.width, size.height);
  ren->begin();
  CCDrawNode* node = CCDrawNode::create();
  node->retain();
  node->setContentSize(size);
  bool rotateLeft = angle<0;
  angle = abs(angle);
  auto h = node->getContentHeight();
  auto left = ccp(0,h);
  auto right = ccp(node->getContentWidth(),h);

  // i still have naming skill issues
  CCPoint last;
  if (rotateLeft) {
    last = right.rotateByAngle(left, -angle);
    float remainingDistRatio = last.x/right.x;
    last = ccp(right.x,last.y*remainingDistRatio);
  } else {
    last = left.rotateByAngle(right, angle);
    float remainingDistRatio = last.x/right.x;
    last = ccp(left.x,last.y/remainingDistRatio);
  }
  CCPoint j[3] = {left, right, last};
  auto color4f = ccc4FFromccc3B(color);
  node->drawPolygon(j, 3, color4f,0,color4f);
  node->drawRect(ccp(0,0), size, color4f,0,color4f);

  node->visit();
  ren->end();
  ren->retain();
  return ren;
}

WaveContainer* WaveContainer::create(ccColor3B color, CCNode* body) {
  create_class(WaveContainer, color, body);
}

bool WaveContainer::init(ccColor3B color, CCNode* pBody) {
  auto s = CCDirector::sharedDirector()->getWinSize();
  auto k = CCSize{s.width*0.8f, s.height};
  this->setContentSize(k);

#define createWave(id, col) \
  wave##id = drawWave(k, col, angle##id); \
  wave##id->setAnchorPoint(ccp(0.5,0)); \
  wave##id->setPosition(ccp(s.width/2,0)); \
  this->addChild(wave##id)

  createWave(1, color);
  createWave(2, color);
  createWave(3, color);
  createWave(4, color);

  body = pBody; // mb
  body->setAnchorPoint(ccp(0.5,0));
  body->setPosition(ccp(s.width/2, 0));
  this->addChild(body);
  body->setContentSize(k);

  return true;
}


void WaveContainer::show() {
  CCDirector::sharedDirector()->getRunningScene()->addChild(this);
  auto h = this->getContentHeight();
  wave1->runAction(CCMoveTo::create(appearDuration, ccp(0,h+(930/1366*h))));
  wave2->runAction(CCMoveTo::create(appearDuration, ccp(0,h+(560/1366*h))));
  wave3->runAction(CCMoveTo::create(appearDuration, ccp(0,h+(390/1366*h))));
  wave4->runAction(CCMoveTo::create(appearDuration, ccp(0,h+(220/1366*h))));
  body->runAction(CCMoveTo::create(appearDuration, ccp(0,h)));

  FMODAudioEngine::sharedEngine()->playEffect("wave-pop-in.wav"_spr);
}

