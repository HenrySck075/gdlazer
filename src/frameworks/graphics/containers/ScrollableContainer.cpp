#include "ScrollableContainer.hpp"

GDL_NS_START

bool ScrollableContainer::init() {
    if (!Container::init()) return false;
    
    // TODO: rectangle clip doesnt work
    setClippingEnabled(false);
    setTouchEnabled(true);
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
    
    // Clamp scroll position
    if (m_scrollDirection == ScrollDirection::Vertical || 
        m_scrollDirection == ScrollDirection::Both) {
        m_scrollPosition.y = std::min(0.0f, 
            std::max(m_scrollPosition.y, 
                    -(contentSize.height - containerSize.height)));
    }
    
    if (m_scrollDirection == ScrollDirection::Horizontal || 
        m_scrollDirection == ScrollDirection::Both) {
        m_scrollPosition.x = std::min(0.0f, 
            std::max(m_scrollPosition.x, 
                    -(contentSize.width - containerSize.width)));
    }
    
    m_content->setPosition(m_scrollPosition);
}

void ScrollableContainer::onMouseDragStart(MouseDragEvent* event) {
    m_scrollVelocityVec = CCPointZero;  // Reset velocity when starting new drag
    if ((m_scrollDirection == ScrollDirection::Vertical || 
         m_scrollDirection == ScrollDirection::Both)) {
        m_scrollPosition.y += event->getDelta().y * m_scrollVelocity;
    } 
    if ((m_scrollDirection == ScrollDirection::Vertical || 
        m_scrollDirection == ScrollDirection::Both)) {
        m_scrollPosition.x += event->getDelta().x * m_scrollVelocity;
    }
    updateChildPosition();
}

void ScrollableContainer::onMouseDragMove(MouseDragEvent* event) {
    if (!m_content) return;
    
    auto delta = event->getDelta();
    if (m_scrollDirection == ScrollDirection::Vertical || 
        m_scrollDirection == ScrollDirection::Both)
        m_scrollPosition.y += delta.y;
        
    if (m_scrollDirection == ScrollDirection::Horizontal || 
        m_scrollDirection == ScrollDirection::Both)
        m_scrollPosition.x += delta.x;
        
    updateChildPosition();
}

void ScrollableContainer::onMouseDragStop(MouseDragEvent* event) {
    m_scrollVelocityVec = event->getDelta();
}

void ScrollableContainer::applyInertia(float dt) {
    if (m_scrollVelocityVec.x != 0 || m_scrollVelocityVec.y != 0) {
        m_scrollPosition = ccpAdd(m_scrollPosition, m_scrollVelocityVec);
        m_scrollVelocityVec = ccpMult(m_scrollVelocityVec, 0.95f);
        
        if (fabsf(m_scrollVelocityVec.x) < 0.1f) m_scrollVelocityVec.x = 0;
        if (fabsf(m_scrollVelocityVec.y) < 0.1f) m_scrollVelocityVec.y = 0;
        
        updateChildPosition();
    }
}

GDL_NS_END
