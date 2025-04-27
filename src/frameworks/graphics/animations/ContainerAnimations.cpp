#include "ContainerAnimations.hpp"
#include "../containers/Container.hpp"
#include "../../utils/Interpolation.hpp"
#include <stdexcept>
GDF_NS_START
namespace animations {
  TintTo* TintTo::create(float duration, cocos2d::ccColor4B color) {
    auto r = new TintTo();
    if (r && r->initWithColor(duration, color)) {
      r->autorelease();
    } else {
      delete r;
      r = nullptr;
    }
    return r;
  }
  bool TintTo::initWithColor(
      float d, cocos2d::ccColor4B c) {
    if (!initWithDuration(d))
      return false;
    m_destColor = c;
    return true;
  }

  void TintTo::startWithTarget(cocos2d::CCNode* target) {
    if (auto c = geode::cast::typeinfo_cast<Container*>(target)) {
      m_startColor = c->getBackgroundColor();
      return CCActionInterval::startWithTarget(target);
    }
    throw std::invalid_argument("Container animations is only supported for container target. You can try cocos2d::CCTintTo instead.");
  }

  void TintTo::update(float dt) {
    cocos2d::CCActionInterval::update(dt);
    if (auto c = geode::cast::typeinfo_cast<Container*>(m_pTarget)) {
#define lerpbloat(v) (GLubyte)gdlazer::frameworks::Interpolation::lerp(m_startColor.v, m_destColor.v, dt)
      c->setBackgroundColor({
        lerpbloat(r),
        lerpbloat(g),
        lerpbloat(b),
        lerpbloat(a)
      });
    };
  }
}
GDF_NS_END
