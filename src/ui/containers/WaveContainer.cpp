#include "WaveContainer.hpp"
#include "../../utils.hpp"
#include <cstdlib>

CCDrawNode* drawWave(CCSize size, ccColor3B color, float angle) {
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
    last = right.rotateByAngle(left, angle);
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

  return node;
  /*
  node->visit();
  ren->end();
  ren->retain();
  return ren;
  */
}

CCDrawNode* WaveContainer::createWave(CCSize size, float angle, ccColor3B col) {
  auto wave = drawWave(size, col, angle); 
  wave->setAnchorPoint(ccp(0.5,0)); 
  wave->setPosition(ccp(size.width/2,0));
  return wave;
}

WaveContainer* WaveContainer::create(ccColor3B color, CCNode* body) {
  auto s = CCDirector::sharedDirector()->getWinSize();
  create_class(WaveContainer, initAnchored, s.width, s.height, color, body);
}

bool WaveContainer::setup(ccColor3B color, CCNode* pBody) {
  m_mainLayer->setVisible(false);
  return customSetup(color, pBody);
}

bool WaveContainer::customSetup(ccColor3B color, CCNode* pBody) {
  auto s = CCDirector::sharedDirector()->getWinSize();
  auto k = CCSize{s.width*0.8f, s.height};
  this->setContentSize(s);


  this->wave1 = createWave(k, angle1, color);
  this->wave2 = createWave(k, angle2, color);
  this->wave3 = createWave(k, angle3, color);
  this->wave4 = createWave(k, angle4, color);

  this->addChild(wave1);
  this->addChild(wave2);
  this->addChild(wave3);
  this->addChild(wave4);

  this->body = pBody; // mb
  body->setAnchorPoint(ccp(0.5,0));
  body->setPosition(ccp(s.width/2, 0));
  this->addChild(body);
  body->setContentSize(k);

  return true;
}


void WaveContainer::show() {
  CCDirector::sharedDirector()->getRunningScene()->addChild(this);
  auto h = this->getContentHeight();
  
#define j(id, dist) wave##id->runAction(CCEaseSineOut::create(CCMoveTo::create(appearDuration, ccp(wave##id->getPositionX(),h+(dist/1366*h)))))
  j(1,930.f/1366*h);
  j(2,560.f/1366*h);
  j(3,390.f/1366*h);
  j(4,220.f/1366*h);
  body->runAction(CCEaseSineOut::create(CCMoveTo::create(appearDuration, ccp(body->getPositionX(),h))));

  FMODAudioEngine::sharedEngine()->playEffect("wave-pop-in.wav"_spr);
}

