/// A collection of CCActionInterval reimplementations to support Container due to polymorphism

#include <Geode/cocos/actions/CCActionInterval.h>

#include "../../../macro.h"

GDF_NS_START
class ContainerMoveTo : public cocos2d::CCActionInterval {
public:
  static ContainerMoveTo* create(float duration, cocos2d::CCPoint position);
  bool init(float duration, cocos2d::CCPoint position);
  void startWithTarget(cocos2d::CCNode* target) override;
  void update(float) override;

private:
  cocos2d::CCPoint m_startPosition;
  cocos2d::CCPoint m_endPosition;
  cocos2d::CCPoint m_deltaPosition;
};

class ContainerTintTo : public cocos2d::CCActionInterval {
public:
  static ContainerTintTo* create(float duration, cocos2d::ccColor4B color);
  bool init(float duration, cocos2d::ccColor4B color);
  void startWithTarget(cocos2d::CCNode* target) override;
  void update(float) override;
protected:
  cocos2d::ccColor4B m_startColor;
  cocos2d::ccColor4B m_endColor;
  cocos2d::ccColor4B m_deltaColor;
};

class ContainerTintOpacityTo : public ContainerTintTo {
public:
  static ContainerTintOpacityTo* create(float duration, GLubyte opacity);
  bool init(float duration, GLubyte opacity);
  void startWithTarget(cocos2d::CCNode* target) override;
private: 
  GLubyte m_opacity;
};

GDF_NS_END