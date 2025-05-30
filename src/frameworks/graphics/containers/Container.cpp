#include "Container.hpp"

#include "../../../utils.hpp"
//#include "../../utils/h2dFormatter.hpp"

GDF_NS_START
void Container::setMinSize(const cocos2d::CCSize &size) {
  assert(m_maxSize.width >= 0 || size.width <= m_maxSize.width);
  assert(m_maxSize.height >= 0 || size.height <= m_maxSize.height);
  m_minSize = size;
}

void Container::setMaxSize(const cocos2d::CCSize &size) {
  assert(m_minSize.width > 0 || size.width >= m_minSize.width);
  assert(m_minSize.height > 0 || size.height >= m_minSize.height);
  m_maxSize = size;
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

void Container::requestBoxUpdate() {
  if (m_containerBoxDesynced) updateContainerBox();
  else if (m_containerBoxShapeDesynced) transformContainerBox();
}

bool Container::init() {
  if (!CCClippingNode::init())
    return false;

  m_backgroundNode = cocos2d::CCLayerColor::create();
  this->addChild(m_backgroundNode, -998);
  
  setStencil(cocos2d::CCDrawNode::create());

  // main handler that processes MouseDown & MouseUp event to others when applicable
  this->addListener<MouseEvent>([this](MouseEvent *mouseEvent) {
    auto currentPos = convertToNodeSpaceA(this, mouseEvent->m_position);
    // request a box update (if needed)
    requestBoxUpdate();
    /*
    geode::log::debug("[{}]: {} {}", getObjectName(this), currentPos, d == nullptr ? h2d::FRectD{
      h2d::Point2dD{0,0},
      h2d::Point2dD{2,2}
    } : d->size() >= 2 ? d->getBB() : h2d::FRectD{
      h2d::Point2dD{0,0},
      h2d::Point2dD{1,1}
    });
    */
    bool isInBounds = true;
    isInBounds = m_containerBox!=nullptr && h2d::Point2dF(
      currentPos.x, currentPos.y
    ).isInside(*(m_containerBox));
    //geode::log::debug("[Container]: {} | {} | {}", isInBounds, currentPos, (intptr_t)m_containerBox);
    if (!isInBounds) {
      //mouseEvent->stopPropagation();
      //if (mouseEvent->m_eventType!=MouseEventType::Move) goto fish;
    }
    switch (mouseEvent->m_eventType) {
      case MouseEventType::MouseUp:
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
        if (m_shouldSendClick) {
          dispatchEvent(new MouseEvent(MouseEventType::Click, mouseEvent->m_position, mouseEvent->m_clicked));
        }
        break;
      case MouseEventType::MouseDown: {
        if (!m_touchEnabled) return false;
        m_shouldSendClick = true;
        m_lastMousePos = currentPos;
        m_dragStartPos = currentPos;  // Store initial position for drag
        if (!isInBounds) goto fish;
        break;
      }
      case MouseEventType::Move: {
        if (m_isDragging) {
          m_lastDragOffset = currentPos - m_lastMousePos;
          dispatchEvent(new MouseDragEvent(
            MouseDragEventType::Move, 
            m_dragStartPos, currentPos, 
            m_lastDragOffset
          ));
          //mouseEvent->stopPropagation();
        } else if (mouseEvent->m_clicked && isInBounds) {
          m_isDragging = true;
          dispatchEvent(new MouseDragEvent(MouseDragEventType::Start, currentPos, currentPos, CCPointZero));
          //mouseEvent->stopPropagation();
        }
        if (m_lastInBounds != isInBounds) {
          dispatchEvent(new MouseEvent(isInBounds ? MouseEventType::Enter : MouseEventType::Exit, currentPos, mouseEvent->m_clicked));
          m_lastInBounds = isInBounds;
          //geode::log::debug("[Container]: Move enter/exit checking for {}: {}", this, isInBounds);
        }
        m_lastMousePos = currentPos;
        break;
      }
      // Everything after this line is generated by the container itself
      case MouseEventType::Click:
        break;
      case MouseEventType::Exit:
        //log::debug("exit | {}", isInBounds);
        if (m_isDragging) {
          m_isDragging = false;
          // should we actually do this?
          dispatchEvent(new MouseDragEvent(MouseDragEventType::Stop, m_dragStartPos, currentPos, m_lastDragOffset));
        }
        mouseEvent->stopPropagation();
        m_lastInBounds = false;
        m_shouldSendClick = false;
        return true;
        break;
      case MouseEventType::Enter:
        //log::debug("enter | {}", isInBounds);
        mouseEvent->stopPropagation();
        m_lastInBounds = true;
        return true;
        break;
    }
fish:
    m_lastInBounds = isInBounds;
    return isInBounds;
  });

  this->addListener<NodeLayoutUpdated>([this](NodeLayoutUpdated* event) {
    updateSizeWithUnit();
    updatePositionWithUnit();
    m_backgroundNode->setContentSize(getContentSize());
    return true;
  });

  scheduleUpdate();
  return true;
}

void Container::drawBorder() {
  auto stencil = geode::cast::typeinfo_cast<CCDrawNode*>(getStencil());
  stencil->clear();
  
  auto size = getContentSize();
  auto radius = m_borderRadius;
  
  if (m_clippingEnabled && stencil == nullptr) {
    /// this is wacky but like i have no idea why it crashes
    stencil = cocos2d::CCDrawNode::create();
    setStencil(stencil);
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
    requestBoxUpdate();
    // Draw corners
    for (int i = 0; i < /*t_vc*/c_verticesPointsSize; i++) {            
      vertices[i] = cocos2d::CCPoint{m_verticesPoints[i][0], m_verticesPoints[i][1]};
    }
    
    if (stencil) stencil->drawPolygon(vertices, c_verticesPointsSize, {1,1,1,1}, 0, {0,0,0,0});

    //delete[] vertices;
  } else {
    cocos2d::CCPoint amougis[4] {
      {0,0},
      {size.width, 0},
      size,
      {0,size.height}
    };
    // Draw rectangle background for no radius
    ccColor4F bgColor = ccc4FFromccc4B(m_backgroundColor);
    if (stencil) stencil->drawPolygon(amougis, 4, bgColor, 0.f, bgColor);
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
  updateSizeWithUnit();
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
void Container::setPosition(cocos2d::CCPoint const& position, Unit hUnit, Unit vUnit) {
  m_position = position;
  m_positionUnit[0] = hUnit;
  m_positionUnit[1] = vUnit;
  updatePositionWithUnit();
}
void Container::setPosition(cocos2d::CCPoint const& position, Unit unit) {
  setPosition(position, unit, unit);
}
void Container::setPosition(cocos2d::CCPoint const& position) {
  setPosition(position, Unit::OpenGL, Unit::OpenGL);
}
void Container::setParent(cocos2d::CCNode *parent) {
  cocos2d::CCNode::setParent(parent);
  dispatchEvent(new NodeLayoutUpdated(parent));
}

void Container::updateSizeWithUnit() {
  float width = m_size.width,
  height = m_size.height;
  
  #define $applyConstraint(edge) \
  if (m_minSize.edge > 0) { \
  edge = std::max(edge, m_minSize.edge); \
  } \
  if (m_maxSize.edge > 0) { \
  edge = std::min(edge, m_maxSize.edge); \
  }

  $applyConstraint(width);
  $applyConstraint(height);
  #undef $applyConstraint

  cocos2d::CCNode::setContentSize(cocos2d::CCSize(
    processUnit(width, m_sizeUnit[0], true),
    processUnit(height, m_sizeUnit[1], false)
  ));
  m_containerBoxDesynced = true;
}

static bool isTopAnchor(geode::Anchor anchor) {
  return anchor == geode::Anchor::Top ||
         anchor == geode::Anchor::TopLeft ||
         anchor == geode::Anchor::TopRight;
}
static bool isRightAnchor(geode::Anchor anchor) {
  return anchor == geode::Anchor::Right ||
         anchor == geode::Anchor::TopRight ||
         anchor == geode::Anchor::BottomRight;
}

void Container::updatePositionWithUnit() {
  cocos2d::CCPoint unanchoredPos {
    processUnit(m_position.x*(isRightAnchor(m_anchor)?-1:1), m_positionUnit[0], true),
    processUnit(m_position.y*(isTopAnchor(m_anchor)?-1:1), m_positionUnit[1], false)
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
  if (!m_containerBoxShapeDesynced && !force) return;
  if (
    getContentSize() == cocos2d::CCSize{0,0} ||
    // doubt it is this
    getContentWidth() < m_borderRadius || getContentHeight() < m_borderRadius
  ) {
    //geode::log::warn("Homographies crash check failed for {}, no container box is created.", this);
    return;
  }
  auto cs2 = getContentSize() - (cocos2d::CCPoint{m_borderRadius, m_borderRadius});
  auto bl = h2d::Point2dF{m_borderRadius, m_borderRadius},
       tr = h2d::Point2dF{cs2.width, cs2.height};
  

  //geode::log::debug("inner rect of {}: {}", this, innerRect);

  if (m_borderRadius > 0) {
    std::set<h2d::Point2dF> p;
    auto size = getContentSize();
    auto radius = m_borderRadius;
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
  //log::debug("{}", p.getPts());
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
  switch (m_anchor) {
  case Anchor::Center:
    return {position.x + size.width / 2, position.y + size.height / 2};
  case Anchor::TopLeft:
    return {position.x, position.y + size.height};
  case Anchor::Top:
    return {position.x + size.width / 2, position.y + size.height};
  case Anchor::TopRight:
    return {position.x + size.width, position.y + size.height};
  case Anchor::Right:
    return {position.x + size.width, position.y + size.height / 2};
  case Anchor::BottomRight:
    return {position.x + size.width, position.y};
  case Anchor::Bottom:
    return {position.x + size.width / 2, position.y};
  case Anchor::Left:
    return {position.x, position.y + size.height / 2};
  case Anchor::BottomLeft:
  default:
    return position;
  }
}
void Container::setAnchor(geode::Anchor anchor) {
  m_anchor = anchor;
  setUserObject("gdlazer/devtools/anchor", cocos2d::CCInteger::create((int)anchor));
  updatePositionWithUnit();
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
