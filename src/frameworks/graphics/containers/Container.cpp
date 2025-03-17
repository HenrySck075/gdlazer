#include "Container.hpp"

GDL_NS_START

void Container::setMinSize(const cocos2d::CCSize &size) {
  assert(m_maxSize.width > 0 || size.width >= m_maxSize.width);
  assert(m_maxSize.height > 0 || size.height >= m_maxSize.height);
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

bool Container::init() {
    if (!CCClippingNode::init())
        return false;

    m_borderNode = cocos2d::CCDrawNode::create();
    this->addChild(m_borderNode, -999);

    m_backgroundNode = cocos2d::CCDrawNode::create();
    this->addChild(m_backgroundNode, -998);

    this->setStencil(cocos2d::CCDrawNode::create());

    this->addListener("mouseEvent", [this](Event *event) {
        auto mouseEvent = static_cast<MouseEvent *>(event);
        auto currentPos = mouseEvent->m_position;

        switch (mouseEvent->m_eventType) {
        case MouseEventType::MouseUp:
            if (m_isDragging) {
                m_isDragging = false;
                onMouseDragStop(new MouseDragEvent(MouseDragEventType::Stop, m_dragStartPos, currentPos, 
                    ccpSub(currentPos, m_lastMousePos)));
            }
            onMouseUp(mouseEvent);
            break;
        case MouseEventType::MouseDown:
            m_lastMousePos = currentPos;
            m_dragStartPos = currentPos;  // Store initial position for drag
            onMouseDown(mouseEvent);
            break;
        case MouseEventType::Move:
            if (m_isDragging) {
                onMouseDragMove(new MouseDragEvent(MouseDragEventType::Move, m_dragStartPos, currentPos, 
                    ccpSub(currentPos, m_lastMousePos)));
            } else if (mouseEvent->m_clicked) {
                m_isDragging = true;
                onMouseDragStart(new MouseDragEvent(MouseDragEventType::Start, currentPos, currentPos, CCPointZero));
            }
            m_lastMousePos = currentPos;
            onMouseMove(mouseEvent);
            break;
        case MouseEventType::Click:
            onMouseClick(mouseEvent);
            break;
        case MouseEventType::Exit:
            if (m_isDragging) {
                m_isDragging = false;
                onMouseDragStop(new MouseDragEvent(MouseDragEventType::Move, m_dragStartPos, currentPos, CCPointZero));
            }
            onMouseExit(mouseEvent);
            break;
        case MouseEventType::Enter:
            onMouseEnter(mouseEvent);
            break;
        }
    });

    this->addListener("nodeLayoutUpdated", [this](Event* event) {
        auto layoutEvent = static_cast<NodeLayoutUpdated*>(event);
        if (layoutEvent->getContainer() == getParent()) {
            updateSizeWithUnit();
            updatePositionWithUnit();
        }
    });

    return true;
}

bool Container::dispatchEvent(Event *event) {
  // Handle the event
  if (!EventTarget::dispatchEvent(event)) {
    return false;
  }

  // Propagate to children
  if (!event->canPropagate()) return true;
  auto children = getChildren();
  if (children) {
    for (auto child : geode::cocos::CCArrayExt<CCNode*>(children)) {
      auto eventTarget = dynamic_cast<EventTarget *>(child);
      if (eventTarget && !eventTarget->dispatchEvent(event)) {
        return false;
      }
    }
  }
  return true;
}

void Container::drawBorder() {
  m_borderNode->clear();
  m_backgroundNode->clear();
  auto stencil = geode::cast::typeinfo_cast<CCDrawNode*>(getStencil());
  if (stencil) stencil->clear();
  
  auto size = getContentSize();
  auto radius = m_borderRadius;
  
  if (radius > 0) {
    if (m_clippingEnabled && stencil == nullptr) {
      /// this is wacky but like i have no idea why it crashes
      stencil = cocos2d::CCDrawNode::create();
      setStencil(stencil);
    }
    const int segments = 20;
    cocos2d::CCPoint* vertices = new cocos2d::CCPoint[segments + 2];
    const float angle = 2.0f * M_PI / segments;
    
    // Draw corners
    for (int i = 0; i <= segments; i++) {
        float x = radius * cosf(i * angle);
        float y = radius * sinf(i * angle);
        
        if (i <= segments/4) {
            vertices[i] = ccp(size.width - radius + x, size.height - radius + y);
        } else if (i <= segments/2) {
            vertices[i] = ccp(radius + x, size.height - radius + y);
        } else if (i <= 3*segments/4) {
            vertices[i] = ccp(radius + x, radius + y);
        } else {
            vertices[i] = ccp(size.width - radius + x, radius + y);
        }
    }
    
    //m_borderNode->drawPolygon(vertices, segments + 1, {0,0,0,0}, 1, {1,1,1,1});
    if (stencil) stencil->drawPolygon(vertices, segments + 1, {1,1,1,1}, 0, {0,0,0,0});
    
    // Draw background
    ccColor4F bgColor = ccc4FFromccc4B(m_backgroundColor);
    m_backgroundNode->drawPolygon(vertices, segments + 1, bgColor, 0, {0,0,0,0});
    
    delete[] vertices;
  } else {
    cocos2d::CCPoint amougis[4] {
      {0,0},
      {size.width, 0},
      size,
      {0,size.height}
    };
    // Draw rectangle background for no radius
    ccColor4F bgColor = ccc4FFromccc4B(m_backgroundColor);
    m_backgroundNode->drawPolygon(amougis, 4, bgColor, 0.f, bgColor);
    setStencil(nullptr);
  }
}

void Container::draw() {
  //drawBorder();
  CCClippingNode::draw();
}

void Container::setBorderRadius(float radius) {
  m_borderRadius = radius;
  drawBorder();
}

void Container::setBackgroundColor(const ccColor4B& color) {
  m_backgroundColor = color;
  drawBorder();
}
void gdlazer::Container::setSize(const cocos2d::CCSize &size, Unit unit) {
  m_size = size;
  m_lastSizeUnit = unit;
  updateSizeWithUnit();
}
void gdlazer::Container::setPosition(cocos2d::CCPoint position, Unit unit) {
  m_positionA = position;
  m_lastPositionUnit = unit;
  updatePositionWithUnit();
}
void gdlazer::Container::setParent(cocos2d::CCNode *parent) {
  cocos2d::CCNode::setParent(parent);
  updateSizeWithUnit();
  updatePositionWithUnit();
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

  setContentSize(cocos2d::CCSize(
    processUnit(width, m_lastSizeUnit, true),
    processUnit(height, m_lastSizeUnit, false)
  ));
  dispatchEvent(new NodeLayoutUpdated(this));
}

void Container::updatePositionWithUnit() {
  CCNode::setPosition(
    processUnit(m_positionA.x, m_lastPositionUnit, true),
    processUnit(m_positionA.y, m_lastPositionUnit, false)
  );
}

void Container::setClippingEnabled(bool enabled) {
  if (m_clippingEnabled == enabled) return;
  m_clippingEnabled = enabled;
  updateClipping();
}

void Container::updateClipping() {
  auto stencil = getStencil();
  if (stencil == nullptr && m_clippingEnabled) stencil = cocos2d::CCDrawNode::create();
  setStencil(m_clippingEnabled ? stencil : nullptr);
  drawBorder();
}
GDL_NS_END