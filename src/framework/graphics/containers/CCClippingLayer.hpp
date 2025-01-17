#pragma once
#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

class IStencilEnabledState {
public:
  virtual bool stencilEnabled() = 0;
};

// j
class CCClippingLayer : public CCLayerColor {
  CCNode* m_pStencil;
  bool m_bInverted = false;
  float m_fAlphaThreshold = 0.01;
public:
  void visit() override;
  bool init(ccColor4B color, CCNode* stencil) {
    if (!CCLayerColor::initWithColor(color)) return false;
    m_pStencil = stencil;
    if (stencil) m_pStencil->retain();
    return true;
  }
  ~CCClippingLayer() {
    if (m_pStencil) m_pStencil->release();
  }
  //static CCClippingLayer* create(float radius) {}
};
