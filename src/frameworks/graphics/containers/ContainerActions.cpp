#include "ContainerActions.hpp"
#include "Container.hpp"

cocos2d::ccColor4B operator+(
    cocos2d::ccColor4B lhs, cocos2d::ccColor4B rhs) {
  return {
    static_cast<GLubyte>(lhs.r + rhs.r),
    static_cast<GLubyte>(lhs.g + rhs.g),
    static_cast<GLubyte>(lhs.b + rhs.b),
    static_cast<GLubyte>(lhs.a + rhs.a)
  };
}
cocos2d::ccColor4B operator+(
    gdlazer::frameworks::ccc4BSigned lhs, cocos2d::ccColor4B rhs) {
  return {
    static_cast<GLubyte>(lhs.r + rhs.r),
    static_cast<GLubyte>(lhs.g + rhs.g),
    static_cast<GLubyte>(lhs.b + rhs.b),
    static_cast<GLubyte>(lhs.a + rhs.a)
  };
}

gdlazer::frameworks::ccc4BSigned operator-(
    cocos2d::ccColor4B lhs, cocos2d::ccColor4B rhs) {
  return {
    lhs.r - rhs.r,
    lhs.g - rhs.g,
    lhs.b - rhs.b,
    lhs.a - rhs.a
  };
}

gdlazer::frameworks::ccc4BSigned operator*(
    gdlazer::frameworks::ccc4BSigned lhs, float rhs) {
  return {
    (int)std::round(lhs.r * rhs),
    (int)std::round(lhs.g * rhs),
    (int)std::round(lhs.b * rhs),
    (int)std::round(lhs.a * rhs)
  };
}

GDF_NS_START
Container* __checkTarget(cocos2d::CCNode* target) {
  auto node = geode::cast::typeinfo_cast<Container*>(target);
  if (!node) {
    throw std::invalid_argument("Container actions is built for Container only.");
  }
  return node;
}

#pragma region ContainerMoveTo
//////////////////////////
/// ContainerMoveTo
//////////////////////////

ContainerMoveTo* ContainerMoveTo::create(
  float duration, cocos2d::CCPoint position
) {
  $createClass(ContainerMoveTo, init, duration, position);
}
bool ContainerMoveTo::init(
  float duration, cocos2d::CCPoint position
) {
  if (!cocos2d::CCActionInterval::initWithDuration(duration))
    return false;

  m_endPosition = position;

  return true;
}
void ContainerMoveTo::startWithTarget(cocos2d::CCNode* target) {
  auto node = __checkTarget(target);
  CCActionInterval::startWithTarget(target);
  m_startPosition = node->m_position;

  m_deltaPosition = m_endPosition - m_startPosition;
}
void ContainerMoveTo::update(float dt) {
  auto node = static_cast<Container*>(m_pTarget);
  node->setPosition(
    m_startPosition + m_deltaPosition*dt,
    node->m_positionUnit[0],
    node->m_positionUnit[1]
  );
}
#pragma endregion

#pragma region ContainerTintTo

//////////////////////////
/// ContainerTintTo
//////////////////////////

ContainerTintTo* ContainerTintTo::create(
  float duration, cocos2d::ccColor4B color
) {
  $createClass(ContainerTintTo, init, duration, color);
}
bool ContainerTintTo::init(
  float duration, cocos2d::ccColor4B color
) {
  if (!cocos2d::CCActionInterval::initWithDuration(duration))
    return false;

  m_endColor = color;

  return true;
}
void ContainerTintTo::startWithTarget(cocos2d::CCNode* target) {
  auto node = __checkTarget(target);
  CCActionInterval::startWithTarget(target);
  m_startColor = node->getBackgroundColor();
  
  m_deltaColor = m_endColor - m_startColor;
}
void ContainerTintTo::update(float dt) {
  static_cast<Container*>(getTarget())->setBackgroundColor(
    m_deltaColor*dt + m_startColor
  );
}

ContainerTintOpacityTo* ContainerTintOpacityTo::create(
  float duration, GLubyte opacity
) {
  $createClass(ContainerTintOpacityTo, init, duration, opacity);
}
bool ContainerTintOpacityTo::init(float d, GLubyte o) {
  m_opacity = o;
  return ContainerTintTo::init(d, {0,0,0,o});
}
void ContainerTintOpacityTo::startWithTarget(cocos2d::CCNode* target) {
  auto node = __checkTarget(target);
  CCActionInterval::startWithTarget(target);
  m_endColor = m_startColor = node->getBackgroundColor();
  m_endColor.a = m_opacity;

  geode::log::debug("[ContainerTintOpacityTo]: {} {}", m_startColor, m_endColor);
  
  m_deltaColor = m_endColor - m_startColor;
}

#pragma endregion

GDF_NS_END