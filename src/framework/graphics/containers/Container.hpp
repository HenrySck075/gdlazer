#pragma once

// we javascripting

#include <Geode/Geode.hpp>
#include "../../bindables/Event.hpp"
#include "../../input/events/MouseEvent.hpp"
#include "../../input/events/MouseDragEvent.hpp"
#include "CCClippingLayer.hpp"
#include "../../bindables/EventTarget.hpp"
#include "../../../utils.hpp"
#include "../Vector4.hpp"
#include "../Color4.hpp"
using namespace geode::prelude;

enum class NodeLayoutUpdateType {
  Position,
  Size,
  // specifically setParent
  All 
};

class NodeLayoutUpdate final : public NodeEvent {
public:
  NodeLayoutUpdateType type;
  NodeLayoutUpdate(NodeLayoutUpdateType t) : NodeEvent("nodeLayoutUpdate"), type(t) {
    setDispatchingFlow(DispatchingFlow::Down);
  };
};


enum class Unit {
  /// Relative to the parent size. Value in range 0-100. Value higher or lower is allowed.
  Percent,
  /// Viewport. Value in range 0-1 with 0 is 0 and 1 is full viewport length. Value higher or lower is allowed.
  Viewport,
  /// OpenGL Point
  OpenGL,
  /// UIKit Point
  UIKit
};

namespace {
  enum class ah {Left, Center, Right};
  enum class av {Top, Center, Bottom};

  struct CCRectExtra : public CCRect {
    
    inline constexpr CCRectExtra(float x, float y, CCSize const& size) {
      setRect(x, y, size.width, size.height);
    }
    inline constexpr CCPoint const& getTopLeft() {
      return CCPoint{getMinX(), getMaxY()};
    }
    inline constexpr CCPoint const& getTopRight() {
      return CCPoint{getMaxX(), getMaxY()};
    }
    inline constexpr CCPoint const& getBottomLeft() {
      return CCPoint{getMinX(), getMinY()};
    }
    inline constexpr CCPoint const& getBottomRight() {
      return CCPoint{getMaxX(), getMinY()};
    }
  };
};

class balls : public CCDrawNode, public IStencilEnabledState {
private:
  float m_radius = 0;
public:
  void drawRoundedRect();
  void drawCircle(const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY, const ccColor4F &color);
  void drawCircle(const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, const ccColor4F &color) {
    drawCircle(center, radius, angle, segments, drawLineToCenter, 1.f, 1.f, color);
  };
  bool stencilEnabled() override;
  bool init(float rad);
  void setContentSize(const CCSize &size) override;
  static balls* create(float rad = 0) {
    $create_class(balls, init, rad);
  }
  void setRadius(float rad);
  float getRadius();
};

class ContainerLayout;
/// @brief CCLayer that implements some more shit
///
/// Works similar to the JavaScript event system
/// 
/// @warning Opacity cascade is ENABLED by default! I spent like almost 1 hour fixing the opacity issue
/// in ToolbarButton and the issue is opacity cascade is enabled
///
/// TODO: Add support for rounded corners
/// TODO: Restrict children to be Containers
class Container : public CCClippingLayer, public EventTarget {
  bool m_entered = false;
  bool m_holding = false;
  bool m_clickEnabled = true;
  bool m_hoverEnabled = true;

  bool m_dragEnabled = false;
  CCPoint mouseDownPos = {0,0};
  // according to osu!framework
  int clickDragDistance = 10;

  MouseDragEvent* currentDragEvent = nullptr;

  balls* m_roundedBorderStencil = nullptr;
public:
  // for those who need to add more checks to when dragging is enabled
  virtual bool dragEnabled() {return m_dragEnabled;}
  void dragEnabled(bool enable) {m_dragEnabled = enable;}
  void initHandler();

  virtual void onMouseEnter() {};
  virtual void onMouseExit() {};
  virtual void onMouseMove(MouseEvent* event) {};
  virtual void onMouseScroll(MouseEvent* event) {};
  virtual void onMouseUp(MouseEvent* event) {};
  virtual void onMouseDown(MouseEvent* event) {};
  virtual void onDragStart(MouseDragEvent* event) {};
  virtual void onDrag(MouseDragEvent* event) {};
  virtual void onDragEnd(MouseDragEvent* event) {};
  virtual void onClick(MouseEvent* event) {};

  void setClickEnabled(bool e);
  bool getClickEnabled();
  void setHoverEnabled(bool state);
  bool getHoverEnabled();

  bool isMouseEntered() {return m_entered;}
  bool isMouseHolding() {return m_holding;}

private:
  NodeEvent* queuedLayoutUpdate = nullptr;

  static std::map<Anchor, std::pair<ah,av>> m_anchors;
  static std::map<Anchor, std::string> m_anchorDebugLabel; 

  std::string getUnitLabel(Unit unit);
  void updateAnchorLabel();
  void updateSizeUnitLabel();
  void updatePositionUnitLabel();

  inline CCSize const& contentSizeWithPadding(CCSize const& size, Vector4 padding);

protected:
  std::string name = "";
  CCSize minimumSize = CCSize(0,0);
  CCSize maximumSize = CCSize(0,0);

  void checkConstraints();

  bool m_includeShadow = false;

  Anchor m_anchor = Anchor::BottomLeft;

  virtual void onLayoutUpdate(NodeLayoutUpdate*e);
  CCPoint m_position;
  CCSize m_size = CCSize(0,0);
  CCSize m_sizeP = CCSize(0,0);
  Vector4 m_padding = {0};

  // first: hori | second: verti
  std::pair<Unit, Unit> m_sizeUnit = std::make_pair(Unit::OpenGL, Unit::OpenGL);
  std::pair<Unit, Unit> m_positionUnit = std::make_pair(Unit::OpenGL, Unit::OpenGL);

  //bool tryDispatch(Callback cb, NodeEvent* event) override;

  /// Dispatch event to children in the given list
  /// 
  /// Useful when you want to dispatch event to childrens from other nodes
  bool dispatchToChildInList(NodeEvent* event, CCArray* children);

  /// made specifically for OverlaysWatcherContainer 
  /// because apparently it does not have an rtti data?
  bool m_ignoreLogging = false;
public:
  std::string getName() {return name;}
  void setRadius(float radius) {
    m_roundedBorderStencil->setRadius(radius);
  }
  void onEnter() override;

  // @param value Value in specified unit
  // @param unit The unit in question
  // @returns The value in OpenGL unit
  float processUnit(float value, Unit unit, bool width);
  
  bool dispatchEvent(NodeEvent* event) override;
  // Dispatches the event to the child. 
  virtual bool dispatchToChild(NodeEvent* event);

  /// nvm
  void updateLayout(bool mpp = false) {
    log::warn("[{} | Container]: Use a FillFlowContainer instead!. I mean, it is better to split the code to multiple classes than squeezing it into one like a beginner's main.cpp, right?", getNodeName(this));
  }

  bool init();

  static Container* create();
  // Sets the position anchor
  void setAnchor(Anchor anchor) {
    m_anchor = anchor;
    updateAnchorLabel();
    dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Position));
  }
  Anchor getAnchor() {
    return m_anchor;
  }
  // get the j
  auto getSizeUnit() {return m_sizeUnit;}

  // set the size unit that will be used to calculate the result size on the next `setContentSize` call
  void setSizeUnit(Unit sizeUnitHorizontal, Unit sizeUnitVertical) {
    m_sizeUnit.first = sizeUnitHorizontal;
    m_sizeUnit.second = sizeUnitVertical;
    updateSizeUnitLabel();
  }

  // set a new content size while also providing the unit that will be parsed
  void setContentSizeWithUnit(CCSize const& size, Unit sizeUnitHorizontal, Unit sizeUnitVertical) {
    setSizeUnit(sizeUnitHorizontal, sizeUnitVertical);
    setContentSize(size);
  };

private:
  /// @returns if the size is changed
  bool resetContentSize();
public:
  void setContentSize(CCSize const& size) override {
    m_size = size;
    resetContentSize();
    dispatchToChild(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
  }
  void setContentWidth(float width) {
    setContentSize(CCSize(width,getContentHeight()));
  }
  void setContentHeight(float height) {
    setContentSize(CCSize(getContentWidth(),height));
  }
  // Get the actual node content size
  const CCSize& getContentSize() const override{
    return m_size;
  }

  void setSizeConstraints(CCSize const& minSize, CCSize const& maxSize);
  std::pair<CCSize, CCSize> getSizeConstraints() {return std::make_pair(minimumSize,maximumSize);};

  void setPadding(Vector4 padding) {
    m_padding = padding;
    dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::All));
  }
  Vector4 const& getPadding() {return m_padding;}

  // set the position unit that will be used to calculate the result position on the next `setPosition` call
  void setPositionUnit(Unit posUnitHorizontal, Unit posUnitVertical) {
    m_positionUnit.first = posUnitHorizontal;
    m_positionUnit.second = posUnitVertical;
    updatePositionUnitLabel();
  }

  void setPositionWithUnit(CCPoint const& position, Unit posUnitHorizontal, Unit posUnitVertical) {
    setPositionUnit(posUnitHorizontal, posUnitVertical);
    setPosition(position);
  }
  void setPosition(CCPoint const& position) override {
    if (position == m_position) return;
    m_position = position;
    dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Position));

  }
  void setPositionY(float pos) override {
    setPosition({m_position.x,pos});
  }
  void setPositionX(float pos) override {
    setPosition({pos,m_position.y});
  }
  // Get the actual node position
  CCPoint const& getPosition() override{
    return m_position;
  }

  void setParent(CCNode *parent) override;

private:
  Color4 m_color4 = {255,255,255,255};
public:
  void setColor(const ccColor3B& color) override;
  void setColor(const Color4& color);
  ccColor3B const& getColor() override {
    return _realColor;
  };
  Color4 const& getColor4() {return m_color4;};
  // the
  void updateColor() override;
};
