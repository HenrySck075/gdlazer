#include "Container.hpp"

#include "../../../utils.hpp"
//#include "../../utils/h2dFormatter.hpp"

GDF_NS_START
void Container::setMinSize(const cocos2d::CCSize &size) {
  assert(m_maxSize.width >= 0 || size.width <= m_maxSize.width);
  assert(m_maxSize.height >= 0 || size.height <= m_maxSize.height);
  m_minSize = size;
  updateSize();
}

void Container::setMaxSize(const cocos2d::CCSize &size) {
  assert(m_minSize.width >= 0 && size.width >= m_minSize.width);
  assert(m_minSize.height >= 0 && size.height >= m_minSize.height);
  m_maxSize = size;
  updateSize();
}

float Container::processUnit(float value, Unit unit, bool isWidth) {
  switch (unit) {
  case Unit::Percent:
    if (getParent() == nullptr) return value;
    return value / 100.0f *
           (isWidth ? getParent()->getContentSize().width
                    : getParent()->getContentSize().height);
  case Unit::Viewport:
    return value *
           (isWidth
                ? cocos2d::CCDirector::sharedDirector()->getWinSize().width
                : cocos2d::CCDirector::sharedDirector()->getWinSize().height);
  case Unit::OpenGL:
    return value;
  case Unit::UIKit:
    return value /
           (isWidth
                ? (cocos2d::CCDirector::sharedDirector()
                       ->getOpenGLView()
                       ->getFrameSize()
                       .width /
                   cocos2d::CCDirector::sharedDirector()->getWinSize().width)
                : (cocos2d::CCDirector::sharedDirector()
                       ->getOpenGLView()
                       ->getFrameSize()
                       .height /
                   cocos2d::CCDirector::sharedDirector()->getWinSize().height));
  default:
    return value;
  }
}
void Container::setMouseEnabled(bool e) {
  if (!e) {
    if (m_isDragging) dispatchEvent(new MouseEvent(MouseEventType::MouseUp, m_lastMousePos, false));
    if (m_isInBounds) dispatchEvent(new MouseEvent(MouseEventType::Exit, m_lastMousePos, false));
  } 
  m_mouseEnabled = e;
};
void Container::requestBoxUpdate() {
  if (m_containerBoxDesynced) updateContainerBox();
  else if (m_containerBoxShapeDesynced) transformContainerBox();
}

bool Container::init() {
  if (!CCClippingNode::init())
    return false;

  m_backgroundNode = cocos2d::CCLayerColor::create();
  CCNode::addChild(m_backgroundNode, -998);
  
  setStencil(m_stencil = cocos2d::CCDrawNode::create());
  // high
  CCNode::addChild(m_border = cocos2d::CCDrawNode::create(), INT_MAX);

  // main handler that processes MouseDown & MouseUp event to others when applicable
  addListener<MouseEvent>([this](MouseEvent *mouseEvent) {
    if (!m_mouseEnabled) return false;
    auto currentPos = convertToNodeSpaceA(this, mouseEvent->m_position);
    // request a box update (if needed)
    requestBoxUpdate();
    bool isInBounds = true;
    isInBounds = m_containerBox!=nullptr && h2d::Point2dF(
      currentPos.x, currentPos.y
    ).isInside(*(m_containerBox));
    //
    if (!isInBounds) {
      //mouseEvent->stopPropagation();
      //if (mouseEvent->m_eventType!=MouseEventType::Move) goto fish;
    }
    switch (mouseEvent->m_eventType) {
      case MouseEventType::MouseUp: {
        if (!m_touchEnabled) return false;
        if (m_isDragging) {
          m_isDragging = false;
          dispatchEvent(new MouseDragEvent(
            MouseDragEventType::Stop, 
            m_dragStartPos, currentPos, 
            m_lastDragOffset
          ));
          mouseEvent->stopPropagation();
        }
        if (m_shouldSendClick && isInBounds) {
          dispatchEvent(new MouseEvent(MouseEventType::Click, mouseEvent->m_position, /*false*/mouseEvent->m_clicked));
          m_shouldSendClick = false;
        } else {
        }
        break;
      }
      case MouseEventType::MouseDown: {
        if (!m_touchEnabled) return false;
        m_shouldSendClick = true;
        m_lastMousePos = currentPos;
        m_dragStartPos = currentPos;  // Store initial position for drag
        if (!isInBounds) goto fish;
        break;
      }
      case MouseEventType::Move: {
        if (m_dragEnabled) {
          if (m_isDragging) {
            m_lastDragOffset = currentPos - m_lastMousePos;
            dispatchEvent(new MouseDragEvent(
              MouseDragEventType::Move, 
              m_dragStartPos, currentPos, 
              m_lastDragOffset
            ));
            mouseEvent->stopImmediatePropagation();
          } else if (m_touchEnabled && m_shouldSendClick && mouseEvent->m_clicked && isInBounds) {
            m_isDragging = true;
            dispatchEvent(new MouseDragEvent(MouseDragEventType::Start, currentPos, currentPos, CCPointZero));
            mouseEvent->stopImmediatePropagation();
            m_shouldSendClick = false;
          }
        }
        if (m_isInBounds != isInBounds) {
          m_isInBounds = isInBounds;
          dispatchEvent(new MouseEvent(isInBounds ? MouseEventType::Enter : MouseEventType::Exit, currentPos, mouseEvent->m_clicked));
          //
        }
        m_lastMousePos = currentPos;
        break;
      }
      // Everything after this line is generated by the container itself
      case MouseEventType::Click: {
        mouseEvent->stopPropagation();
        break;
      }
      case MouseEventType::Exit: {
        if (m_isDragging) {
          m_isDragging = false;
          // should we actually do this?
          dispatchEvent(new MouseDragEvent(MouseDragEventType::Stop, m_dragStartPos, currentPos, m_lastDragOffset));
        }
        mouseEvent->stopPropagation();
        m_isInBounds = false;
        m_shouldSendClick = false;
        return true;
      }
      case MouseEventType::Enter: {
        mouseEvent->stopPropagation();
        m_isInBounds = true;
        return true;
      }
    }
fish:
    m_isInBounds = isInBounds;
    return isInBounds;
  });

  addListener<NodeLayoutUpdated>([this](NodeLayoutUpdated* event) {
    updateSize();
    updatePosition();
    m_backgroundNode->setContentSize(getContentSize());
    return true;
  });

  scheduleUpdate();
  return true;
}

void Container::drawBorder() {
  auto size = getContentSize();

  bool redrawStencil = (
    size != m_copyOfTheContentSizeInCaseTheOriginalGetsCalled &&
    m_borderRadius.isUpdated()
  );
  bool redrawBorder = (
    m_borderRadius.isUpdated() &&
    m_borderThickness.isUpdated()
  );

  if (redrawStencil) {
    m_copyOfTheContentSizeInCaseTheOriginalGetsCalled = size;
  }
  
  float radius = m_borderRadius;
  float thickness = m_borderThickness;

  if (m_clippingEnabled && m_stencil == nullptr) {
    /// this is wacky but like i have no idea why it crashes
    m_stencil = cocos2d::CCDrawNode::create();
    setStencil(m_stencil);
  }
  if (radius > 0 && size != CCSizeZero) {
    /*
    if (t_vc >= c_verticesPointsSize) {
      t_vc = 3;
      t_fc = 0;
    }
    cocos2d::CCPoint* vertices = new cocos2d::CCPoint[t_vc];
    */
    cocos2d::CCPoint vertices[c_verticesPointsSize];
    if (redrawStencil) requestBoxUpdate();
    // Draw corners
    for (int i = 0; i < /*t_vc*/c_verticesPointsSize; i++) {            
      vertices[i] = cocos2d::CCPoint{m_verticesPoints[i][0], m_verticesPoints[i][1]};
    }
    
    if (m_stencil && redrawStencil) {
      m_stencil->clear();
      m_stencil->drawPolygon(vertices, c_verticesPointsSize, {1,1,1,1}, 0, {0,0,0,0});
    }
    
    // Draw the border
    if (redrawBorder) {
      m_border->clear();
      // Create a copy of all the vertices and move it towards the center by m_borderThickness/2 unit
      cocos2d::CCPoint borderVertices[c_verticesPointsSize];
      for (int i = 0; i < c_verticesPointsSize; i++) {
        borderVertices[i] = cocos2d::CCPoint{
          vertices[i].x - (vertices[i].x - size.width / 2) * m_borderThickness/2 / radius,
          vertices[i].y - (vertices[i].y - size.height / 2) * m_borderThickness/2 / radius
        };
      }
      // actually draw it
      ccColor4F borderColor = ccc4FFromccc4B(m_borderColor);
      m_border->drawPolygon(borderVertices, c_verticesPointsSize, {0,0,0,0}, thickness, borderColor);
    }

  } else {
    cocos2d::CCPoint amougis[4] {
      {0,0},
      {size.width, 0},
      size,
      {0,size.height}
    };
    // Draw rectangle background for no radius
    ccColor4F bgColor = ccc4FFromccc4B(m_backgroundColor);
    if (m_stencil) m_stencil->drawPolygon(amougis, 4, bgColor, 0.f, bgColor);
    if (redrawBorder) {
      m_border->clear();
      ccColor4F borderColor = ccc4FFromccc4B(m_borderColor);
      m_border->drawPolygon(amougis, 4, {0,0,0,0}, thickness, borderColor);
    }
  }
}

void Container::visit() {
  //t_vc++;
  //if (t_vc%24 == 0) t_fc++;
  if (m_clippingEnabled) {
    cocos2d::CCClippingNode::visit();
    drawBorder();
  } else {
    cocos2d::CCNode::visit();
  }
}

void Container::setBorderRadius(float radius) {
  m_borderRadius = radius;
  m_containerBoxShapeDesynced = true;
}

void Container::setBackgroundColor(const ccColor4B& color) {
  m_backgroundColor = color;
  m_backgroundNode->setColor({color.r,color.g,color.b});
  m_backgroundNode->setOpacity(getDisplayedOpacity()/255 * color.a);
}

void Container::setContentSize(const cocos2d::CCSize &size, Unit hUnit, Unit vUnit) {
  m_size = size;
  m_sizeUnit[0] = hUnit;
  m_sizeUnit[1] = vUnit;
  updateSize();
}
void Container::setContentSize(const cocos2d::CCSize &size, Unit unit) {
  setContentSize(size, unit, unit);
}
void Container::setContentSize(const cocos2d::CCSize &size) {
  /// technically this wont happen but idk why tf it did on ScrollContainer
  if (m_vfuncCallLoopBlock) {
    cocos2d::CCNode::setContentSize(size);
    m_containerBoxShapeDesynced = true;
    return;
  }
  m_vfuncCallLoopBlock = true;
  setContentSize(size, Unit::OpenGL, Unit::OpenGL);
  m_vfuncCallLoopBlock = false;
};
void Container::setContentWidth(float width, Unit unit) {
  m_size.width = width;
  m_sizeUnit[0] = unit;
  updateSize();
}
void Container::setContentWidth(float width) {
  setContentWidth(width, Unit::OpenGL);
}
void Container::setContentHeight(float height, Unit unit) {
  m_size.height = height;
  m_sizeUnit[1] = unit;
  updateSize();
}
void Container::setContentHeight(float height) {
  setContentHeight(height, Unit::OpenGL);
}

void Container::setPadding(const Vector4 &padding) {
  m_padding = padding;
  updateSize();
  updatePosition();
}

void Container::setPosition(cocos2d::CCPoint const& position, Unit hUnit, Unit vUnit) {
  m_position = position;
  m_positionUnit[0] = hUnit;
  m_positionUnit[1] = vUnit;
  updatePosition();
}
void Container::setPosition(cocos2d::CCPoint const& position, Unit unit) {
  setPosition(position, unit, unit);
}
void Container::setPosition(cocos2d::CCPoint const& position) {
  setPosition(position, Unit::OpenGL, Unit::OpenGL);
}
void Container::setPositionX(float x, Unit unit) {
  m_position.x = x;
  m_positionUnit[0] = unit;
  updatePosition();
}
void Container::setPositionX(float x) {
  setPositionX(x, Unit::OpenGL);
}
void Container::setPositionY(float y, Unit unit) {
  m_position.y = y;
  m_positionUnit[1] = unit;
  updatePosition();
}
void Container::setPositionY(float y) {
  setPositionY(y, Unit::OpenGL);
}

void Container::setParent(cocos2d::CCNode *parent) {
  cocos2d::CCNode::setParent(parent);
  dispatchEvent(new NodeLayoutUpdated());
}

void Container::updateSize() {
  float width = m_size.width;
  float height = m_size.height;
  
  #define $applyConstraint(edge, isWidth) \
  if (m_minSize.edge >= 0) { \
  edge = std::max(edge, processUnit(m_minSize.edge, Unit::UIKit, isWidth)); \
  } \
  if (m_maxSize.edge > 0) { \
  edge = std::min(edge, processUnit(m_maxSize.edge, Unit::UIKit, isWidth)); \
  }

  $applyConstraint(width, true);
  $applyConstraint(height, false);
  #undef $applyConstraint

  cocos2d::CCNode::setContentSize({
    processUnit(width, m_sizeUnit[0], true) - processUnit(m_padding.l + m_padding.r, Unit::UIKit, true),
    processUnit(height, m_sizeUnit[1], false) - processUnit(m_padding.t + m_padding.d, Unit::UIKit, false)
  });
  m_containerBoxShapeDesynced = true;
  updateContainerBox();
}

void Container::updatePosition() {
  cocos2d::CCPoint unanchoredPos {
    processUnit(m_position.x, m_positionUnit[0], true),
    processUnit(m_position.y, m_positionUnit[1], false)
  };
  setUserObject("gdlazer/devtools/position", cocos2d::CCArray::create(
    cocos2d::CCInteger::create(unanchoredPos.x),
    cocos2d::CCInteger::create(unanchoredPos.y),
    nullptr
  ));
  cocos2d::CCNode::setPosition(calculateAnchoredPosition(unanchoredPos));
}

void Container::setClippingEnabled(bool enabled) {
  //if (m_clippingEnabled == enabled) return;
  m_clippingEnabled = enabled;
  updateClipping();
}

void Container::updateClipping() {
  /*
  if (m_clippingEnabled) {
    setStencil(cocos2d::CCDrawNode::create());
  } else {
    setStencil(nullptr);
  }
  */
}
struct LogNestManager {
  LogNestManager() {
    geode::log::pushNest(geode::Mod::get());
  }
  ~LogNestManager() {
    geode::log::popNest(geode::Mod::get());
  }
};
bool Container::propagateToChildren(CCArray* children, Event* event, std::type_index type) {
  if (children == nullptr) return true;
  geode::Ref<Event> eventRefHolder(event);
  for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode>(children)) {
    if (child == nullptr) {
      log::warn("[{}]: The container contains a nullptr child.", getObjectName(this));
      continue;
    };
    auto childContainer = geode::cast::typeinfo_cast<Container*>(child);
    if (childContainer == nullptr) {
      if (!propagateToChildren(child->getChildren(), event, type)) return false;
      if (event->m_propagateStopped) {
        return true;
      }
      continue;
    }
    else if (!childContainer->doDispatchEvent(event, type)) {
      return false;
    }
  }
  return true;
}
bool Container::doDispatchEvent(Event* event, std::type_index type) {
  geode::Ref<Event> eventRefHolder(event);
  LogNestManager logNest;
  // Handle the event
  if (!EventTarget::doDispatchEvent(event, type)) {
    return false;
  }

  if (event->m_propagateStopped) {
    return true;
  }
  // Propagate to children
  return propagateToChildren(getChildren(), event, type);
};

template<typename T>
h2d::Point2d_<T> operator+(h2d::Point2d_<T> lhs, h2d::Point2d_<T> rhs) {
  return {
    lhs.getX()+rhs.getX(),
    lhs.getY()+rhs.getY()
  };
}

void Container::calculatePolygonVertPoints() {
  float width = getContentWidth();
  float height = getContentHeight();
  /// Mathematically counter-clockwise
  float corners[4][2] = {
    {width-m_borderRadius, height-m_borderRadius},
    {m_borderRadius, height-m_borderRadius},
    {m_borderRadius, m_borderRadius},
    {width-m_borderRadius, m_borderRadius},
  };
  for (int corner = 0; corner < 4; corner++) {
    for (int i = 0; i <= c_segments; i++) {
      int dex = i + (c_segments * corner) + corner;
      float angle = (M_PI / 2 / c_segments) * (i + (c_segments * corner));

      m_verticesPoints[dex][0] = m_borderRadius * cosf(angle) + corners[corner][0];
      m_verticesPoints[dex][1] = m_borderRadius * sinf(angle) + corners[corner][1];
    }
  }
}

/*
template<typename T>
/// Skews the polygon inplace.
/// @param hf horizontal shear factor for each point (how much x changes relative to ydist from origin)
/// @param vf vertical shear factor for each point (how much y changes relative to xdist from origin)
void skewPolygon(h2d::CPolyline_<T> polygon, h2d::Point2d_<T> origin, float hf, float vf) {
  std::vector<h2d::Point2d_<T>> points;
  std::vector<h2d::Point2d_<T>> originalPoints = polygon.getPts();
  points.reserve(originalPoints.size());

  for (auto p : originalPoints) {
    // do the math
    T tx = p.getX() - origin.getX();
    T ty = p.getY() - origin.getY();

    T sx = tx + hf*ty;
    T sy = ty + vf*tx;

    points.emplace_back(sx + origin.getX(), sy + origin.getY());
  }

  polygon.set(points);
}
*/
//[[clang::optnone]]
void Container::transformContainerBox() {
  if (!m_containerBoxDesynced) return;

  /// TODO: the homographies currently demolishes the entire polyline thus making the container box empty
  /// it has been replaced with the original box for now
  // homographies transformation
  h2d::HomogrF h;
  h.addRotation(getRotation()*M_PI/180)
  .addScale(getScaleX(), getScaleY());
  // skewing
  //h.set(0, 1, getSkewX());
  //h.set(1, 0, getSkewY());

  auto mo = (h * *(m_containerBoxO));
  m_containerBox = std::make_unique<h2d::CPolylineF>(mo);
  //m_containerBox = m_containerBoxO;
  m_containerBoxDesynced = false;
}
//[[clang::optnone]]
void Container::updateContainerBox(bool force) {
  float borderRadius = m_borderRadius;
  if (!m_containerBoxShapeDesynced && !force) return;
  if (
    getContentSize() == cocos2d::CCSize{0,0} ||
    // doubt it is this
    getContentWidth() < borderRadius || getContentHeight() < borderRadius
  ) {
    //geode::log::warn("Homographies crash check failed for {}, no container box is created.", this);
    return;
  }
  auto cs2 = getContentSize() - (cocos2d::CCPoint{borderRadius, borderRadius});
  auto bl = h2d::Point2dF{borderRadius, borderRadius},
       tr = h2d::Point2dF{cs2.width, cs2.height};
  

  //

  if (borderRadius > 0) {
    std::set<h2d::Point2dF> p;
    auto size = getContentSize();
    auto radius = borderRadius;
    std::array<h2d::Point2dF, c_verticesPointsSize> vertices;

    calculatePolygonVertPoints();

    for (int i = 0; i < c_verticesPointsSize; i++) {
      vertices[i] = {m_verticesPoints[i][0], m_verticesPoints[i][1]};
    }

    p.clear();
    p.insert(vertices.begin(), vertices.end());
    if (p.size() == 1) return;

    std::vector<h2d::Point2dF> t;
    t.assign(p.begin(), p.end());
    m_containerBoxO = std::make_unique<h2d::CPolylineF>(t);
  /// if it is not a circle
  } else if (!h2d::detail::shareCommonCoord(bl, tr)) {
    h2d::FRectF innerRect {
      bl, tr
    };
    m_containerBoxO = std::make_unique<h2d::CPolylineF>(innerRect);
  } else {
    m_containerBoxO = std::make_unique<h2d::CPolylineF>();
  }

  m_containerBoxShapeDesynced = false;
  m_containerBoxDesynced = true;

  transformContainerBox();
  //
};
void Container::setScale(float scale) {
  CCNode::setScale(scale);
  m_containerBoxDesynced = true;
}
void Container::setScaleX(float scaleX) {
  CCNode::setScaleX(scaleX);
  m_containerBoxDesynced = true;
}
void Container::setScaleY(float scaleY) {
  CCNode::setScaleY(scaleY);
  m_containerBoxDesynced = true;
}
void Container::setSkewX(float skewX) {
  CCNode::setSkewX(skewX);
  m_containerBoxDesynced = true;
};
void Container::setSkewY(float skewY) {
  CCNode::setSkewY(skewY);
  m_containerBoxDesynced = true;
};
void Container::setRotation(float rotation) {
  CCNode::setRotation(rotation);
  m_containerBoxDesynced = true;
}

cocos2d::CCPoint Container::calculateAnchoredPosition(
  const cocos2d::CCPoint &position) {
  using namespace geode;
  auto size = getParent() ? getParent()->getContentSize() : CCSize{0,0};
  // Add padding from m_padding

  // As each of the padding size is only used once, we can use a macro
  // to limit the amount of calculations it has to do to whatever used
  // (re gamers wont like this)
  // , although it will still be calculated twice
  #define padL processUnit(m_padding.l, Unit::UIKit, true)
  #define padT processUnit(m_padding.t, Unit::UIKit, false)
  #define padR processUnit(m_padding.r, Unit::UIKit, true)
  #define padD processUnit(m_padding.d, Unit::UIKit, false)
  switch (m_anchor) {
    case Anchor::Center:
      return {position.x + size.width / 2 + (padL - padR) / 2, position.y + size.height / 2 + (padT - padD) / 2};
    case Anchor::TopLeft:
      return {position.x + padL, size.height - position.y - padT};
    case Anchor::Top:
      return {position.x + size.width / 2 + (padL - padR) / 2, size.height - position.y - padT};
    case Anchor::TopRight:
      return {size.width - position.x - padR, size.height - position.y - padT};
    case Anchor::Right:
      return {size.width - position.x - padR, position.y + size.height / 2 + (padT - padD) / 2};
    case Anchor::BottomRight:
      return {size.width/* - position.x - padR*/, position.y + padD};
    case Anchor::Bottom:
      return {position.x + size.width / 2 + (padL - padR) / 2, position.y + padD};
    case Anchor::Left:
      return {position.x + padL, position.y + size.height / 2 + (padT - padD) / 2};
    case Anchor::BottomLeft:
    default:
      return {position.x + padL, position.y + padD};
  }
  #undef padL
  #undef padT
  #undef padR
  #undef padD
}
void Container::setAnchor(geode::Anchor anchor) {
  m_anchor = anchor;
  setUserObject("gdlazer/devtools/anchor", cocos2d::CCInteger::create((int)anchor));
  updatePosition();
}
void Container::updateDisplayedOpacity(GLubyte parentOpacity) {
  CCClippingNodeRGBA::updateDisplayedOpacity(parentOpacity);
  m_backgroundNode->setOpacity(getDisplayedOpacity()/255*m_backgroundColor.a);
}

void Container::removeAllChildrenWithCleanup(bool cleanup) {
  CCNode::removeAllChildrenWithCleanup(cleanup);
  CCNode::addChild(m_backgroundNode);
}
void Container::setOpacity(GLubyte opacity) {
  CCClippingNodeRGBA::setOpacity(opacity);
  m_backgroundNode->setOpacity(opacity/255*m_backgroundColor.a);
}
GDF_NS_END
