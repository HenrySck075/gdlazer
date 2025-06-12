#include "ScrollContainer.hpp"
#include "../../input/events/MouseDragEvent.hpp"
#include "../../input/events/MouseEvent.hpp"

GDF_NS_START

bool ScrollContainer::init(Container* content) {
    if (!Container::init()) return false;
    
    setClippingEnabled(true);
    setTouchEnabled(true);
    setDragEnabled(true);
    
    // Set content and initialize its position/anchor
    addChild(m_content = content);
    m_content->setAnchor(Anchor::TopLeft);
    m_content->setAnchorPoint({0,1});
    
    /*
    // Initial position at top-left (0,0 in UI coordinates, which is (0,height) in cocos coordinates)
    if (content->getContentSize().height > getContentSize().height) {
        m_scrollPosition.y = -(content->getContentSize().height - getContentSize().height);
    } else {
        m_scrollPosition.y = 0;
    }
*/
    updatePosition();

    this->addListener<MouseScrollEvent>([this](MouseScrollEvent* event){
        if (!m_content) return false;

        if ((m_scrollDirection == ScrollDirection::Vertical)) {
            // Invert scroll delta for natural scrolling direction
            //m_scrollVelocityVec.y = -event->m_scrollDelta.y;
        } else if (m_scrollDirection == ScrollDirection::Horizontal) {
            m_scrollVelocityVec.x = event->m_scrollDelta.y;
        }
        return true;
    });

    this->addListener<MouseDragEvent>([this](MouseDragEvent* event){
        if (!m_content) return false;

        switch (event->m_type) {
            case MouseDragEventType::Start: {
                m_scrollVelocityVec = cocos2d::CCPoint{0,0};
                m_returningInBounds = false;
                m_velocityFriction = 0.95f;
                return true;
            }
            case MouseDragEventType::Move: {                
                auto delta = event->getDelta();
                if (m_scrollDirection == ScrollDirection::Vertical) {
                    m_scrollPosition.y -= delta.y;
                }
                    
                if (m_scrollDirection == ScrollDirection::Horizontal) {
                    m_scrollPosition.x += delta.x;
                }
                
                updatePosition();
                return true;
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
                m_scrollVelocityVec.y = -m_scrollVelocityVec.y;

                schedule(schedule_selector(ScrollContainer::applyInertia));
                return true;
            }
        };
        return true;
    });

    return true;
}

void ScrollContainer::setContent(Container* content) {
    if (m_content != nullptr) {
        removeChild(m_content);
    }
    m_content = content;
    cocos2d::CCNode::addChild(content);
    updatePosition();
}

void ScrollContainer::resizeToChildSize() {
    if (m_content) {
        setContentSize(m_content->getContentSize());
    }
}

void ScrollContainer::updateSize() {
    Container::updateSize();
    if (m_content) {
        updatePosition();
    }
}

void ScrollContainer::updatePosition() {
    if (!m_content) return;
  
    auto containerSize = getContentSize();
    auto contentSize = m_content->getContentSize();

    // Clamp scroll position based on direction
    /*
    if (m_scrollDirection == ScrollDirection::Vertical) {
        if (contentSize.height > containerSize.height) {
            // Convert from UI coordinates (top-down) to cocos coordinates (bottom-up)
            // When scrolling down, m_scrollPosition.y becomes more negative
            m_scrollPosition.y = std::min(-(contentSize.height - containerSize.height), 
                                        std::max(m_scrollPosition.y, 0.0f));
        } else {
            // If content is smaller than container, position at top
            m_scrollPosition.y = -(contentSize.height - containerSize.height);
        }
    }

    if (m_scrollDirection == ScrollDirection::Horizontal) {
        if (contentSize.width > containerSize.width) {
            m_scrollPosition.x = std::min(0.0f, 
                std::max(m_scrollPosition.x, 
                        -(contentSize.width - containerSize.width)));
        } else {
            m_scrollPosition.x = 0;
        }
    }
*/
    //log::debug("[ScrollContainer]: scrollpos: {}", m_scrollPosition);
    
    m_content->setPosition(m_scrollPosition);
}

void ScrollContainer::applyInertia(float dt) {
    m_scrollPosition = m_scrollPosition + m_scrollVelocityVec;
    m_scrollVelocityVec = m_scrollVelocityVec * m_velocityFriction;
    
    if (fabsf(m_scrollVelocityVec.x) < 0.1f) m_scrollVelocityVec.x = 0;
    if (fabsf(m_scrollVelocityVec.y) < 0.1f) m_scrollVelocityVec.y = 0;

    auto contentPos = m_content->getPosition();
    auto contentSize = m_content->getContentSize();
    auto thisContentSize = getContentSize();

    bool leftBoundsL = contentPos.x < 0;
    bool leftBoundsT = contentPos.y < 0;
    bool leftBoundsR = contentPos.x + contentSize.width < thisContentSize.width;
    bool leftBoundsB = contentPos.y + contentSize.height < thisContentSize.height;

    bool oob = !m_returningInBounds && (
        leftBoundsL || leftBoundsT || leftBoundsR || leftBoundsB
    );
    if (oob) {
        m_velocityFriction = 0.7f;
    }
    if (m_scrollVelocityVec == cocos2d::CCPoint{0,0}) {
        if (oob) {
            cocos2d::CCPoint dist{0,0};
            if ((m_scrollDirection == ScrollDirection::Horizontal || 
                m_scrollDirection == ScrollDirection::Both)) {
                if (leftBoundsL) {
                    dist.x = -contentPos.x;
                } else if (leftBoundsR) {
                    dist.x = -(contentPos.x + contentSize.width - thisContentSize.width);
                }
            }
            if ((m_scrollDirection == ScrollDirection::Vertical || 
                m_scrollDirection == ScrollDirection::Both)) {
                if (leftBoundsT) {
                    dist.y = -(contentPos.y + contentSize.height - thisContentSize.height);
                } else if (leftBoundsB) {
                    dist.y = -contentPos.y;
                }
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

void ScrollContainer::scrollBy(cocos2d::CCPoint const& dist, bool animate) {
    if (animate) {
        m_scrollVelocityVec = cocos2d::CCPoint{
            guessVelocityFromDistance(dist.x),
            guessVelocityFromDistance(dist.y)
        };
        schedule(schedule_selector(ScrollContainer::applyInertia));
    } else {
        m_scrollPosition += dist;
        updatePosition();
    }
}

void ScrollContainer::scrollTo(cocos2d::CCPoint const& pos, bool animate) {
    if (animate) {
        m_scrollVelocityVec = cocos2d::CCPoint{
            guessVelocityFromDistance(pos.x - m_scrollPosition.x),
            guessVelocityFromDistance(pos.y - m_scrollPosition.y)
        };
        schedule(schedule_selector(ScrollContainer::applyInertia));
    } else {
        m_scrollPosition = pos;
        updatePosition();
    }
}

GDF_NS_END
