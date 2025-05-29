#include "ContainerActions.hpp"
#include "Container.hpp"

GDF_NS_START
ContainerMoveTo* ContainerMoveTo::create(
  float duration, cocos2d::CCPoint position
) {
  $createClass(ContainerMoveTo, initWithDuration, duration, position);
}
bool ContainerMoveTo::initWithDuration(
  float duration, cocos2d::CCPoint position
) {
  if (!cocos2d::CCActionInterval::initWithDuration(duration))
    return false;

  m_endPosition = position;

  return true;
}
void ContainerMoveTo::startWithTarget(cocos2d::CCNode* target) {
  auto node = geode::cast::typeinfo_cast<Container*>(target);
  if (!node) {
    throw std::invalid_argument("Container actions is built for Container only.");
  }
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

GDF_NS_END