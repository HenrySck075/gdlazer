#pragma once

#include <Geode/Geode.hpp>
#include <typeindex>
#include "../../bindables/EventTarget.hpp"
#include "../../bindables/Event.hpp"

#include "../../../macro.h"

template<typename T>
struct h2dShapeContainer;

GDF_NS_START

class Container;

class NodeLayoutUpdated : public Event {
public:
    NodeLayoutUpdated(Container* container) 
        : m_container(container) {}

    Container* getContainer() const { return m_container; }

private:
    Container* m_container;
};


class Container : public cocos2d::CCClippingNode, public EventTarget {
public:
  enum class Unit {
    Percent,
    Viewport,
    OpenGL,
    UIKit
  };

  static Container* create() {
    Container* ret = new Container();
    if (ret && ret->init()) {
      ret->autorelease();
      return ret;
    } else {
      delete ret;
      return nullptr;
    }
  }
  
  bool init() override;

  // Mouse events
  inline bool getTouchEnabled() {return m_touchEnabled;};
  inline void setTouchEnabled(bool e) {m_touchEnabled = e;};
  // General-use unit conversion function
  float processUnit(float value, Unit unit, bool isWidth);

  // Constraints
  void setMinSize(const cocos2d::CCSize &size);
  void setMaxSize(const cocos2d::CCSize &size);

  // Setters and getters for name
  void setName(const std::string& containerName) { m_name = containerName; }
  const std::string& getName() const { return m_name; }

  void setSize(const cocos2d::CCSize &size, Unit unit = Unit::OpenGL);
  void setContentSize(const cocos2d::CCSize &size) override {
    CCNode::setContentSize(size);
    updateContainerBox();
  };

  // Position setters with units
  void setPosition(cocos2d::CCPoint position, Unit unit = Unit::OpenGL);

  void setParent(cocos2d::CCNode *parent) override;

  bool doDispatchEvent(Event *event, std::type_index type) override;

  void setBorderRadius(float radius);
  float getBorderRadius() {return m_borderRadius;};

  void setBackgroundColor(const cocos2d::ccColor4B& color);
  const cocos2d::ccColor4B& getBackgroundColor() const { return m_backgroundColor; }

  bool getClippingEnabled() const { return m_clippingEnabled; }
  void setClippingEnabled(bool enabled);
  void updateContainerBox();

protected:
  void updateClipping();
  void drawBorder();
  void visit() override;
  virtual void updateSizeWithUnit();
  void updatePositionWithUnit();

private:
  bool m_touchEnabled = false;
  std::string m_name;
  cocos2d::CCSize m_minSize;
  cocos2d::CCSize m_maxSize;
  bool m_isDragging = false;
  cocos2d::CCPoint m_lastMousePos;
  cocos2d::CCPoint m_lastDragOffset;
  cocos2d::CCPoint m_dragStartPos;
  float m_borderRadius = 0.0f;
  cocos2d::CCDrawNode* m_borderNode = nullptr;
  cocos2d::CCDrawNode* m_backgroundNode = nullptr;
  cocos2d::ccColor4B m_backgroundColor = {0, 0, 0, 0};
  cocos2d::CCSize m_size;
  Unit m_lastSizeUnit = Unit::OpenGL;
  cocos2d::CCPoint m_positionA;
  Unit m_lastPositionUnit = Unit::OpenGL;
  bool m_clippingEnabled = true;
  /// whether or not the mouse was in bounds prior to the current mouse event
  bool m_lastInBounds = false;
  /// used to determine if we're going to accept the input
  /// h2dShapeContainer not included to prevent including the entirely of homog2d
  h2dShapeContainer<float>* m_containerBox = nullptr;
};

GDF_NS_END
