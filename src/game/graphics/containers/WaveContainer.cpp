#include "WaveContainer.hpp"
#include "../../../frameworks/graphics/animations/ContainerActions.hpp"
#include "../../../macro.h"
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

GDL_NS_START
using namespace frameworks;
static const float c_angle1 = 13;
static const float c_angle2 = -7;
static const float c_angle3 = 4;
static const float c_angle4 = -2;
CCDrawNode* createWave(float w, CCSize size, float angle, ccColor4B col, std::optional<float> offsety = std::nullopt) {
  CCDrawNode* wave;
  float offset;
  std::tie(wave, offset) = drawWave(size, col, angle); 
  wave->setAnchorPoint({0.5,1}); 
  wave->setPosition({w,offsety.value_or(-offset-2)});
  return wave;
}
WaveContainer* WaveContainer::create(ColorScheme color) {
  $createClass(WaveContainer, init, color);
}

bool WaveContainer::init(ColorScheme color) {
  if (!OverlayContainer::init()) return false;
  addListener<MouseEvent>([this](MouseEvent* e){
    if (e->m_eventType == MouseEventType::MouseDown) {
      m_startTouchLocation = e->m_position;
    }
    else if (e->m_eventType == MouseEventType::Click) {
      if (
        !m_touchBoundary.containsPoint(m_startTouchLocation) && 
        !m_touchBoundary.containsPoint(e->m_position)
      ) hide();
    }

    return true;
  }); 
  addListener<NodeSizeUpdated>([this](NodeSizeUpdated* e) {
    if (!m_shown) return true;
    if (m_wave1 == nullptr) return true;
    removeChild(m_wave1);
    removeChild(m_wave2);
    removeChild(m_wave3);
    removeChild(m_wave4);
    CCSize waveSize = m_useContainerSizeForWaves ? getContentSize() : m_main->getContentSize();
    addChild(m_wave1 = createWave(getContentWidth() / 2, waveSize, c_angle1, m_provider->Light4(), getContentHeight()));
    addChild(m_wave2 = createWave(getContentWidth() / 2, waveSize, c_angle2, m_provider->Light3(), getContentHeight()));
    addChild(m_wave3 = createWave(getContentWidth() / 2, waveSize, c_angle3, m_provider->Dark4(), getContentHeight()));
    addChild(m_wave4 = createWave(getContentWidth() / 2, waveSize, c_angle4, m_provider->Dark3(), getContentHeight()));

    return true;
  });
  setBackgroundColor({0,0,0,static_cast<GLubyte>(255*0.2)});
  m_provider = OverlayColorProvider::create(color);
  m_provider->retain();
  
  //m_touchBoundary = CCRect((s.width-k.width)/2,0,k.width,k.height);
  m_main->setContentSize({80,100}, Unit::Percent);
  setContentSize({100,100}, Unit::Percent);
  m_main->setAnchor(Anchor::Bottom);
  //auto w = s.width/2;

  m_main->setAnchorPoint({0.5,1});
  m_main->setZOrder(777);
  m_main->setPadding({16, 0});
  m_main->setBackgroundColor(m_provider->Dark3());

  return true;
}


void WaveContainer::onOpen() {
  /// As Unit::Percent is dependent on the parent 
  /// and this still hasn't had the overlay container parent yet,
  //queueInMainThread([this]{
    auto h = getContentHeight();
    if (!m_wave1) {
      auto w = m_useContainerSizeForWaves ? getContentWidth() : m_main->getContentWidth();
      auto tw = getContentWidth();
      CCSize k {w,h};
      

      m_wave1 = createWave(tw/2,k, c_angle1, m_provider->Light4());
      m_pos1 = m_wave1->getPositionY();
      m_wave2 = createWave(tw/2,k, c_angle2, m_provider->Light3());
      m_pos2 = m_wave2->getPositionY();
      m_wave3 = createWave(tw/2,k, c_angle3, m_provider->Dark4());
      m_pos3 = m_wave3->getPositionY();
      m_wave4 = createWave(tw/2,k, c_angle4, m_provider->Dark3());
      m_pos4 = m_wave4->getPositionY();
      m_main->setPositionY(m_pos4-15);

      addChild(m_wave1);
      addChild(m_wave2);
      addChild(m_wave3);
      addChild(m_wave4);
    }
    
  #define j(id, dist) \
    m_wave##id->runAction(CCEaseSineOut::create(CCMoveTo::create(m_appearDuration, {m_wave##id->getPositionX(),h+(dist/1366*h)})))
    j(1,930.f);
    j(2,560.f);
    j(3,390.f);
    j(4,220.f);
    m_main->runAction(CCEaseSineOut::create(ContainerMoveTo::create(m_appearDuration, {0,h})));

    FMODAudioEngine::sharedEngine()->playEffect("wave-pop-in.wav"_spr);
  #undef j
  //});
}

void WaveContainer::onClose() {
  // nuh uh
  stopAllActions();

#define j(id) m_wave##id->runAction(CCEaseSineIn::create(CCMoveTo::create(m_disappearDuration, ccp(m_wave##id->getPositionX(),m_pos##id))))
  j(1);
  j(2);
  j(3);
  j(4);
  m_main->runAction(CCEaseSineIn::create(ContainerMoveTo::create(m_disappearDuration, {0,m_pos4-15})));
  runAction(CCDelayTime::create(m_disappearDuration));

  FMODAudioEngine::sharedEngine()->playEffect("overlay-big-pop-out.wav"_spr);
#undef j
}
GDL_NS_END