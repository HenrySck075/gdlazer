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

    this->addListener<MouseEvent>([this](MouseEvent *mouseEvent) {
      auto currentPos = mouseEvent->m_position;
      if (!boundingBox().containsPoint(currentPos)) {
        mouseEvent->stopPropagation();
        return false;
      }

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
              onMouseDragStop(new MouseDragEvent(MouseDragEventType::Stop, m_dragStartPos, currentPos, CCPointZero));
          }
          onMouseExit(mouseEvent);
          break;
        case MouseEventType::Enter:
          onMouseEnter(mouseEvent);
          break;
      }
      return true;
    });

    this->addListener<NodeLayoutUpdated>([this](NodeLayoutUpdated* event) {
      if (event->getContainer() == getParent()) {
        updateSizeWithUnit();
        updatePositionWithUnit();
      }
      return true;
    });

    return true;
}

void Container::drawBorder() {
  m_borderNode->clear();
  m_backgroundNode->clear();
  auto stencil = geode::cast::typeinfo_cast<CCDrawNode*>(getStencil());
  stencil->clear();
  
  auto size = getContentSize();
  auto radius = m_borderRadius;
  
  if (m_clippingEnabled && stencil == nullptr) {
    /// this is wacky but like i have no idea why it crashes
    stencil = cocos2d::CCDrawNode::create();
    setStencil(stencil);
  }
  if (radius > 0) {
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
    
    m_borderNode->drawPolygon(vertices, segments + 1, {0,0,0,0}, 1, {1,1,1,1});
    stencil->drawPolygon(vertices, segments + 1, {1,1,1,1}, 0, {0,0,0,0});
    
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
    stencil->drawPolygon(amougis, 4, bgColor, 0.f, bgColor);
  }
}

void Container::draw() {
  drawBorder();
  CCClippingNode::draw();
}

void Container::setBorderRadius(float radius) {
  m_borderRadius = radius;
}

void Container::setBackgroundColor(const ccColor4B& color) {
  m_backgroundColor = color;
}
void Container::setSize(const cocos2d::CCSize &size, Unit unit) {
  m_size = size;
  m_lastSizeUnit = unit;
  updateSizeWithUnit();
}
void Container::setPosition(cocos2d::CCPoint position, Unit unit) {
  m_positionA = position;
  m_lastPositionUnit = unit;
  updatePositionWithUnit();
}
void Container::setParent(cocos2d::CCNode *parent) {
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
  if (m_clippingEnabled) drawBorder();
}
bool Container::doDispatchEvent(Event *event, std::type_index type) {
  geode::Ref<Event> eventRefHolder(event);
  // crashes somewhere after printing this
  geode::log::debug("Dispatching {} to {}'s handlers", getObjectName(event),
                    getObjectName(this));
  // Handle the event
  if (!EventTarget::doDispatchEvent(event, type)) {
    return false;
  }

  // Propagate to children
  if (event->m_propagateStopped) {
    return true;
  }
  geode::log::pushNest(geode::Mod::get());
  auto children = getChildren();
  if (children) {
    for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode>(children)) {
      // casting directly to Container makes it use this overridden
      // dispatchEvent function
      auto childContainer = geode::cast::typeinfo_cast<EventTarget*>(child);
      if (childContainer != nullptr)
        continue;
      if (!childContainer->dispatchEvent(event)) {
        geode::log::popNest(geode::Mod::get());
        return false;
      }
    }
  }
  geode::log::popNest(geode::Mod::get());
  // idk i think this would keep the object around
  auto idk = eventRefHolder.data();
  return true;
};
GDL_NS_END
