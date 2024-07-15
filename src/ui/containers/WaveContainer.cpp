#include "WaveContainer.hpp"
#include "../../utils.hpp"
#include "ccTypes.h"
#include <cstdlib>

CCDrawNode* drawWave(ccColor3B color, float angle) {
  CCDrawNode* node = CCDrawNode::create();
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
  node->drawRect(ccp(0,0), node->getContentSize(), color4f,0,color4f);

  return node;
}

WaveContainer* WaveContainer::create(ccColor3B color, CCNode* body) {
  create_class(WaveContainer, color, body);
}

bool WaveContainer::init(ccColor3B color, CCNode* pBody) {
  auto s = CCDirector::sharedDirector()->getWinSize();
  this->setContentSize(CCSize{s.width*0.8f, s.height});

#define createWave(id, col) \
  wave##id = drawNodeToSprite(drawWave(col, angle##id)); \
  wave##id->setAnchorPoint(ccp(s.width/2,0)); \
  this->addChild(wave##id)

  createWave(1, color);
  createWave(2, color);
  createWave(3, color);
  createWave(4, color);

  body = pBody; // mb
  body->setAnchorPoint(ccp(s.width/2,0));
  this->addChild(body);
  body->setContentSize(this->getContentSize());

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

