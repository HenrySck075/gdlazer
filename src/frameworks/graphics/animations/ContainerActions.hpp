/// A collection of CCActionInterval reimplementations to support Container due to polymorphism
#pragma once
#include <Geode/cocos/actions/CCActionInterval.h>

#include "../../../macro.h"

GDF_NS_START

struct ccc4BSigned {
  int r;
  int g;
  int b;
  int a;
};


/** @brief Sizes a CCNode object to a zoom factor by modifying it's contentSize attribute.
 @warning This action doesn't support "reverse"
 */
class ContainerResizeTo : public cocos2d::CCActionInterval {
public:
  /** initializes the action with the same scale factor for X and Y */
  bool initWithDuration(float duration, float s);

  /** initializes the action with and X factor and a Y factor */
  bool initWithDuration(float duration, float sx, float sy);
  /**
   *  @js NA
   *  @lua NA
   */
  //virtual CCObject* copyWithZone(CCZone* pZone);
  void startWithTarget(cocos2d::CCNode* pTarget) override;
  void update(float time) override;

public:

  /** creates the action with the same scale factor for X and Y */
  static ContainerResizeTo* create(float duration, float s);

  /** creates the action with and X factor and a Y factor */
  static ContainerResizeTo* create(float duration, float sx, float sy);
protected:
  float m_fContentWidth;
  float m_fContentHeight;
  float m_fStartContentWidth;
  float m_fStartContentHeight;
  float m_fEndContentWidth;
  float m_fEndContentHeight;
  float m_fDeltaWidth;
  float m_fDeltaHeight;

  bool m_isContainer;
};


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
  ccc4BSigned m_deltaColor;
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