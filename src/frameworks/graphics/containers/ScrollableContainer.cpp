#include "ScrollableContainer.hpp"
#include "../../input/events/MouseDragEvent.hpp"
GDF_NS_START

bool ScrollableContainer::init() {
  if (!Container::init()) return false;
  
  // TODO: rectangle clip doesnt work
  setClippingEnabled(false);
  setTouchEnabled(true);

  this->addListener<MouseDragEvent>([this](MouseDragEvent* event){
    switch (event->m_type) {
    case MouseDragEventType::Start: {
      m_scrollVelocityVec = cocos2d::CCPoint{0,0};
      m_returningInBounds = false;
      m_velocityFriction = 0.95f;
      if ((m_scrollDirection == ScrollDirection::Vertical || 
           m_scrollDirection == ScrollDirection::Both)) {
          m_scrollPosition.y += event->getDelta().y * m_scrollVelocity;
      } 
      if ((m_scrollDirection == ScrollDirection::Vertical || 
          m_scrollDirection == ScrollDirection::Both)) {
          m_scrollPosition.x += event->getDelta().x * m_scrollVelocity;
      }
      updateChildPosition();
      break;
    }
    case MouseDragEventType::Move: {
      if (!m_content) break;
      
      auto delta = event->getDelta();
      if (m_scrollDirection == ScrollDirection::Vertical || 
          m_scrollDirection == ScrollDirection::Both)
          m_scrollPosition.y += delta.y;
          
      if (m_scrollDirection == ScrollDirection::Horizontal || 
          m_scrollDirection == ScrollDirection::Both)
          m_scrollPosition.x += delta.x;
          
      updateChildPosition();
      break;
    }
    case MouseDragEventType::Stop: {
      m_scrollVelocityVec = event->getDelta();

      if (m_scrollDirection == ScrollDirection::Vertical || 
          m_scrollDirection == ScrollDirection::None) {
        m_scrollVelocityVec.x = 0;
      }
      if (m_scrollDirection == ScrollDirection::Horizontal || 
          m_scrollDirection == ScrollDirection::None) {
        m_scrollVelocityVec.y = 0;
      }
      schedule(schedule_selector(ScrollableContainer::applyInertia));
      break;
    }
    };
  });
  return true;
}

void ScrollableContainer::addChild(CCNode* child) {
  addChild(child, 0);
}

void ScrollableContainer::addChild(CCNode* child, int zOrder) {
  addChild(child, zOrder, 0);
}

void ScrollableContainer::addChild(CCNode* child, int zOrder, int tag) {
  if (m_content != nullptr) {
    removeChild(m_content);
  }
  m_content = child;
  Container::addChild(child, zOrder, tag);
  updateChildPosition();
}

void ScrollableContainer::resizeToChildSize() {
  if (m_content) {
    setContentSize(m_content->getContentSize());
  }
};

void ScrollableContainer::updateSizeWithUnit() {
  Container::updateSizeWithUnit();
  if (m_content) {
    updateChildPosition();
  }
}

void ScrollableContainer::updateChildPosition() {
  if (!m_content) return;
  
  auto containerSize = getContentSize();
  auto contentSize = m_content->getContentSize();
  
  /*
  // Clamp scroll position
  m_scrollPosition.y = std::min(0.0f, 
      std::max(m_scrollPosition.y, 
              -(contentSize.height - containerSize.height)));
  m_scrollPosition.x = std::min(0.0f, 
      std::max(m_scrollPosition.x, 
              -(contentSize.width - containerSize.width)));
  */
  m_content->setPosition(m_scrollPosition);
}

void ScrollableContainer::applyInertia(float dt) {
  if (m_scrollVelocityVec.x != 0 || m_scrollVelocityVec.y != 0) {
    m_scrollPosition = ccpAdd(m_scrollPosition, m_scrollVelocityVec);
    m_scrollVelocityVec = ccpMult(m_scrollVelocityVec, m_velocityFriction);
    
    if (fabsf(m_scrollVelocityVec.x) < 0.1f) m_scrollVelocityVec.x = 0;
    if (fabsf(m_scrollVelocityVec.y) < 0.1f) m_scrollVelocityVec.y = 0;

    auto contentPos = m_content->getPosition();
    auto contentSize = m_content->getContentSize();
    auto thisContentSize = getContentSize();

    bool leftBoundsL = contentPos.x < 0;
    bool leftBoundsT = contentPos.y < 0;
    bool leftBoundsR = contentPos.x + contentSize.width > thisContentSize.width;
    bool leftBoundsB = contentPos.y + contentSize.height > thisContentSize.height;

    bool oob = !m_returningInBounds && (
      leftBoundsL || leftBoundsT || leftBoundsR || leftBoundsB
    );
    if (oob) {
      m_velocityFriction = 0.7f;
    }
    if (m_scrollVelocityVec == cocos2d::CCPoint{0,0}) {
      if (oob) {
        cocos2d::CCPoint dist{0,0};
        if (contentPos.x < 0) {
          dist.x = -contentPos.x;
        } else if (leftBoundsR) {
          dist.x = -(contentPos.x + contentSize.width - thisContentSize.width);
        }
        if (contentPos.y < 0) {
          dist.y = -contentPos.y;
        } else if (leftBoundsB) {
          dist.y = -(contentPos.y + contentSize.height - thisContentSize.height);
        }

        m_scrollVelocityVec = cocos2d::CCPoint{
          guessVelocityFromDistance(dist.x),
          guessVelocityFromDistance(dist.y)
        };

        m_returningInBounds = true;
      }
      else {
        unschedule(schedule_selector(ScrollableContainer::applyInertia));
      }
    }
    
    updateChildPosition();
  }
}

GDF_NS_END
