#include "Container.hpp"
float Container::processUnit(float value, Unit unit, bool isWidth) {
  switch (unit) {
  case Unit::Percent:
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
  if (!CCLayer::init())
    return false;

  m_borderNode = cocos2d::CCDrawNode::create();
  this->addChild(m_borderNode, -999);

  this->addListener("MouseEvent", [this](Event *event) {
    auto mouseEvent = static_cast<MouseEvent *>(event);
    auto currentPos = mouseEvent->m_position;

    switch (mouseEvent->m_eventType) {
    case MouseEventType::MouseUp:
      if (m_isDragging) {
        m_isDragging = false;
        onMouseDragStop(new MouseDragEvent("dragStop", m_dragStartPos, currentPos, 
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
        onMouseDragMove(new MouseDragEvent("dragMove", m_dragStartPos, currentPos, 
            ccpSub(currentPos, m_lastMousePos)));
      } else if (mouseEvent->m_clicked) {
        m_isDragging = true;
        onMouseDragStart(new MouseDragEvent("dragStart", currentPos, currentPos, CCPointZero));
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
        onMouseDragStop(new MouseDragEvent("dragMove", m_dragStartPos, currentPos, CCPointZero));
      }
      onMouseExit(mouseEvent);
      break;
    case MouseEventType::Enter:
      onMouseEnter(mouseEvent);
      break;
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
    CCObject *child;
    CCARRAY_FOREACH(children, child) {
      auto eventTarget = dynamic_cast<EventTarget *>(child);
      if (eventTarget && !eventTarget->dispatchEvent(event)) {
        return false;
      }
    }
  }
  return true;
}

void Container::updateClipping() {
    if (m_clipChildren) {
        if (!m_clipNode) {
            m_clipNode = cocos2d::CCClippingNode::create();
            m_clipStencil = cocos2d::CCDrawNode::create();
            m_clipNode->setStencil(m_clipStencil);
            
            // Move all children to clip node
            auto children = CCLayer::getChildren();
            if (children) {
                CCObject* child;
                CCARRAY_FOREACH(children, child) {
                    if (child != m_borderNode && child != m_clipNode) {
                        m_clipNode->addChild(static_cast<CCNode*>(child));
                    }
                }
            }
            
            CCLayer::addChild(m_clipNode);
        }
    } else if (m_clipNode) {
        // Move all children back to main container
        auto children = m_clipNode->getChildren();
        if (children) {
            CCObject* child;
            CCARRAY_FOREACH(children, child) {
                CCLayer::addChild(static_cast<CCNode*>(child));
            }
        }
        
        m_clipNode->removeFromParent();
        m_clipNode = nullptr;
        m_clipStencil = nullptr;
    }
    
    drawBorder();
}

void Container::drawBorder() {
    m_borderNode->clear();
    if (m_clipStencil) m_clipStencil->clear();
    
    auto size = getContentSize();
    auto radius = m_borderRadius;
    
    if (radius > 0) {
        const int segments = 20;
        cocos2d::CCPoint* vertices = new cocos2d::CCPoint[segments + 2];
        const float angle = 2.0f * M_PI / segments;
        
        // Draw corners
        for (int i = 0; i <= segments; i++) {
            float x = radius * cosf(i * angle);
            float y = radius * sinf(i * angle);
            
            if (i <= segments/4) { // Top-right
                vertices[i] = ccp(size.width - radius + x, size.height - radius + y);
            } else if (i <= segments/2) { // Top-left
                vertices[i] = ccp(radius + x, size.height - radius + y);
            } else if (i <= 3*segments/4) { // Bottom-left
                vertices[i] = ccp(radius + x, radius + y);
            } else { // Bottom-right
                vertices[i] = ccp(size.width - radius + x, radius + y);
            }
        }
        
        m_borderNode->drawPolygon(vertices, segments + 1, ccc4f(0,0,0,0), 1, ccc4f(1,1,1,1));
        if (m_clipStencil) {
            m_clipStencil->drawPolygon(vertices, segments + 1, ccc4f(1,1,1,1), 0, ccc4f(0,0,0,0));
        }
        
        delete[] vertices;
    }
}

void Container::draw() {
    CCLayer::draw();
    drawBorder();
}
void Container::setClipChildren(bool clip) {
  m_clipChildren = clip;
  updateClipping();
}
void Container::setBorderRadius(float radius) {
  m_borderRadius = radius;
  updateClipping();
}
