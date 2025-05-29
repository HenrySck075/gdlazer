/// A collection of CCActionInterval reimplementations to support Container due to polymorphism

#include <Geode/cocos/actions/CCActionInterval.h>

#include "../../../macro.h"

GDF_NS_START
class ContainerMoveTo : public cocos2d::CCActionInterval {
public:
  static ContainerMoveTo* create(float duration, cocos2d::CCPoint position);
  bool initWithDuration(float duration, cocos2d::CCPoint position);
  void startWithTarget(cocos2d::CCNode *target) override;
  void update(float) override;

private:
  cocos2d::CCPoint m_startPosition;
  cocos2d::CCPoint m_endPosition;
  cocos2d::CCPoint m_deltaPosition;
};

GDF_NS_END