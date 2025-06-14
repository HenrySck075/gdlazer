#pragma once

#include <Geode/Geode.hpp>
#include "CCClippingNodeRGBA.hpp"
#include <memory>
#include <typeindex>
#include "../../bindables/EventTarget.hpp"
#include "../../bindables/Event.hpp"
#include "../../../macro.h"
#include "../../utils/homog2d.hpp"
#include "../Vector4.hpp"

/// Included for free
#include "../../input/events/MouseEvent.hpp"
#include "../../input/events/MouseDragEvent.hpp"
#include "../../input/events/KeyEvent.hpp"


GDF_NS_START
class NodeLayoutUpdated : public Event {
public:
    NodeLayoutUpdated() {}
};

/// cant figure out a name for this one
template<std::copyable T>
struct _ModifiedStateValue {
private:
  T m_state;
  bool m_updated = false;
public:
  _ModifiedStateValue() = default;
  _ModifiedStateValue(T state) : m_state(state) {}

  /// Some will scream, some won't
  T& operator->() {
    return m_state;
  }
  operator T() {return m_state;}
  _ModifiedStateValue<T> operator=(const T& state) {
    m_state = state;
    m_updated = true;
    return *this;
  }

  inline bool isUpdated() const { return m_updated; }
  inline void resetUpdateState() { m_updated = false; }
};

enum class Unit {
  Percent,
  Viewport,
  OpenGL,
  UIKit
};
/// HOLY SHIT
class Container : public CCClippingNodeRGBA, public EventTarget {
public:

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
  inline bool getDragEnabled() {return m_dragEnabled;};
  inline void setDragEnabled(bool e) {m_dragEnabled = e;};
  inline bool getMouseEnabled() {return m_mouseEnabled;};
  void setMouseEnabled(bool e);
  inline bool isMouseEntered() {return m_isInBounds;}
  // General-use unit conversion function
  float processUnit(float value, Unit unit, bool isWidth);

  // Constraints
  void setMinSize(const cocos2d::CCSize &size);
  void setMaxSize(const cocos2d::CCSize &size);

  // Setters and getters for name
  void setName(const std::string& containerName) { m_name = containerName; }
  const std::string& getName() const { return m_name; }

  // Size setters
  void setContentSize(const cocos2d::CCSize &size, Unit unit);
  void setContentSize(const cocos2d::CCSize &size, Unit hUnit, Unit vUnit);
  void setContentSize(const cocos2d::CCSize &size) override;
  // and its single-axis functions
  void setContentWidth(float width, Unit unit);
  void setContentWidth(float width); // only works if called directly with the Container type
  void setContentHeight(float height, Unit unit);
  void setContentHeight(float height); // only works if called directly with the Container type
  // padding
  void setPadding(const Vector4 &padding);
  inline const Vector4& getPadding() {return m_padding;}

  // Position setters
  void setPosition(cocos2d::CCPoint const& position) override;
  void setPosition(cocos2d::CCPoint const& position, Unit unit);
  void setPosition(cocos2d::CCPoint const& position, Unit hUnit, Unit vUnit);
  // and its single-axis functions
  void setPositionX(float x) override;
  void setPositionX(float x, Unit unit);
  void setPositionY(float y) override;
  void setPositionY(float y, Unit unit);

  // function overrides to mark m_containerBoxDesynced to true
  void setScale(float scale) override;
  void setScaleX(float scaleX) override;
  void setScaleY(float scaleY) override;
  void setSkewX(float skewX) override;
  void setSkewY(float skewY) override;
  void setRotation(float rotation) override;

  void setParent(cocos2d::CCNode* parent) override;

  bool doDispatchEvent(Event* event, std::type_index type) override;
  virtual bool propagateToChildren(CCArray* children, Event* event, std::type_index type);

  void setBorderRadius(float radius);
  float getBorderRadius() {return m_borderRadius;};

  void setBackgroundColor(const cocos2d::ccColor4B& color);
  const cocos2d::ccColor4B& getBackgroundColor() const { return m_backgroundColor; }

  bool getClippingEnabled() const { return m_clippingEnabled; }
  void setClippingEnabled(bool enabled);
  void updateContainerBox(bool force = false);
  void transformContainerBox();

  // Anchor point
  void setAnchor(geode::Anchor anchor);

  cocos2d::CCPoint calculateAnchoredPosition(const cocos2d::CCPoint &position);
  geode::Anchor getAnchor() const { return m_anchor; }

  void updateDisplayedOpacity(GLubyte parentOpacity) override;

  void removeAllChildrenWithCleanup(bool cleanup) override;
  void setOpacity(GLubyte opacity) override;

protected:
  void updateClipping();
  void drawBorder();
  void visit() override;
  virtual void updateSize();
  void updatePosition();
  void calculatePolygonVertPoints();
  void requestBoxUpdate();

private:
  geode::Anchor m_anchor = geode::Anchor::BottomLeft;

  /// new naming: t_ for temp (debugging) variables
  //int t_fc = 0;
  //int t_vc = 3;
  bool m_vfuncCallLoopBlock = false;

  CCSize m_copyOfTheContentSizeInCaseTheOriginalGetsCalled;

  static const int c_segments = 20;
  static const int c_verticesPointsSize = c_segments * 4 + 4;
  std::array<std::array<float,2>, c_verticesPointsSize> m_verticesPoints; friend class DebugDrawNode;
  geode::Ref<cocos2d::CCDrawNode> m_stencil;
  geode::Ref<cocos2d::CCDrawNode> m_border;
  _ModifiedStateValue<float> m_borderThickness = 0;
  _ModifiedStateValue<float> m_borderRadius = 0.0f;
  geode::Ref<cocos2d::CCLayerColor> m_backgroundNode;
  cocos2d::ccColor4B m_backgroundColor = {0, 0, 0, 0};
  cocos2d::ccColor4B m_borderColor = {0, 0, 0, 0};

  bool m_touchEnabled = false;
  bool m_dragEnabled = false;
  bool m_mouseEnabled = true;
  std::string m_name;
  cocos2d::CCSize m_minSize;
  cocos2d::CCSize m_maxSize;
  bool m_isDragging = false;
  bool m_shouldSendClick = false;
  cocos2d::CCPoint m_lastMousePos;
  cocos2d::CCPoint m_lastDragOffset;
  cocos2d::CCPoint m_dragStartPos;
  /// As we have a copy of the size, this should be good enough.
  /// Paddings are in UIKit size
  Vector4 m_padding;
  cocos2d::CCSize m_size;                                      friend class ContainerResizeTo;
  Unit m_sizeUnit[2] {Unit::OpenGL, Unit::OpenGL};
  cocos2d::CCPoint m_position;                                      friend class ContainerMoveTo;
  Unit m_positionUnit[2] {Unit::OpenGL, Unit::OpenGL};
  bool m_clippingEnabled = false;
  /// whether or not the mouse was in bounds prior to the current mouse event
  bool m_isInBounds = false;

  /// the untransformed box to later apply the transformation
  std::unique_ptr<h2d::CPolylineF> m_containerBoxO = nullptr;
  /// used to determine if we're going to accept the input
  /// type is h2d::CPolylineF but its not included to prevent including the entirely of homog2d
  std::unique_ptr<h2d::CPolylineF> m_containerBox = nullptr;

  /// this one causes a container box retransform (everything else)
  bool m_containerBoxDesynced = true;
  /// this one causes a container box rebuild (true on borderRadius and size change)
  bool m_containerBoxShapeDesynced = true;
};

GDF_NS_END
