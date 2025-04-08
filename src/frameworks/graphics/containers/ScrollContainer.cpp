#include "ScrollContainer.hpp"
#include "../../input/events/MouseDragEvent.hpp"
#include "../../input/events/MouseEvent.hpp"
GDF_NS_START

bool ScrollContainer::init(Container* content) {
  if (!Container::init()) return false;
  
  // TODO: rectangle clip doesnt work
  setClippingEnabled(false);
  setTouchEnabled(true);

  m_content = content;

  this->addListener<MouseScrollEvent>([this](MouseScrollEvent* event){
    if ((m_scrollDirection == ScrollDirection::Vertical || 
         m_scrollDirection == ScrollDirection::Both)
    ) {
      m_scrollVelocityVec.y = event->m_scrollDelta.y;
    } else {
      // quite controversal impl but idk
      m_scrollVelocityVec.x = event->m_scrollDelta.y;
    }
    return true;
  });
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
      updatePosition();
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
          
      updatePosition();
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
      schedule(schedule_selector(ScrollContainer::applyInertia));
      break;
    }
    };
    return true;
  });
  return true;
}

void ScrollContainer::addChild(cocos2d::CCNode* child) {
  addChild(child, 0);
}

void ScrollContainer::addChild(cocos2d::CCNode* child, int zOrder) {
  addChild(child, zOrder, 0);
}

void ScrollContainer::addChild(cocos2d::CCNode* child, int zOrder, int tag) {
  geode::log::warn(
    "[ScrollContainer]: "
    "As I only support 1 child in my entire lifetime (excluding the background and scrollbar), "
    "please use `ScrollContainer::setScrollChild` to not make confusions for anyone reading the code."
  );
  setContent(child);
}

void ScrollContainer::setContent(cocos2d::CCNode* child) {
  m_vfuncCallLoopBlock = true;
  if (m_content != nullptr) {
    removeChild(m_content);
  }
  m_content = child;
  cocos2d::CCNode::addChild(child); // still calls the override but to make it less confusing
  updatePosition();
  m_vfuncCallLoopBlock = false;
}

void ScrollContainer::resizeToChildSize() {
  if (m_content) {
    setContentSize(m_content->getContentSize());
  }
};

void ScrollContainer::updateSizeWithUnit() {
  Container::updateSizeWithUnit();
  if (m_content) {
    updatePosition();
  }
}

void ScrollContainer::updatePosition() {
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

void ScrollContainer::applyInertia(float dt) {
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

        scrollBy(dist, true);
        m_returningInBounds = true;
      }
      else {
        unschedule(schedule_selector(ScrollContainer::applyInertia));
      }
    }
    
    updatePosition();
  }
}

void ScrollContainer::scrollBy(cocos2d::CCPoint const& dist, bool animate) {
  if (animate) {
    m_scrollVelocityVec = cocos2d::CCPoint{
      guessVelocityFromDistance(dist.x),
      guessVelocityFromDistance(dist.y)
    };
  } else {
    m_scrollPosition += dist;
    updatePosition();
  }
};
void ScrollContainer::scrollTo(cocos2d::CCPoint const& pos, bool animate) {
  if (animate) {
    m_scrollVelocityVec = cocos2d::CCPoint{
      guessVelocityFromDistance(pos.x - m_scrollPosition.x),
      guessVelocityFromDistance(pos.y - m_scrollPosition.y)
    };
  } else {
    m_scrollPosition = pos;
    updatePosition();
  }
};
//////////////////////////////////////////////////
/// ScrollbarContainer
//////////////////////////////////////////////////

bool ScrollbarContainer::init(ScrollContainer* scrollContainer) {
  setContentSize(scrollContainer->getContentSize());
  setPosition(scrollContainer->getPosition());
  scrollContainer->setPosition({0,0});
};
GDF_NS_END
