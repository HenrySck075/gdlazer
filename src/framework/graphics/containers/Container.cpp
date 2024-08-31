#include "Container.hpp"
#include "../CCPointExtensions.hpp"
#include "../../input/events/MouseEvent.hpp"

void InputHandlerImpl::initHandler() {
    addListener("mouseEvent",[this](NodeEvent* e) {
        auto event = static_cast<MouseEvent*>(e);
        auto type = event->eventType;
        //if (type == MouseEventType::Move) event->logging(true);
        if (!(this->m_hoverEnabled && isRunning())) return;
        switch (type) {
            case MouseEventType::Move:
                this->onMouseMove(event);
                break;
            case MouseEventType::Enter:
                m_entered = true;
                this->onMouseEnter();
                break;
            case MouseEventType::Exit:
                m_entered = false;
                this->onMouseExit();
                break;
            case MouseEventType::MouseUp:
                if (m_holding) {
                    this->onMouseUp(event);
                    if (dragEnabled() && currentDragEvent) {
                        onDragEnd(currentDragEvent);
                        currentDragEvent->release();
                        currentDragEvent = nullptr;
                    }
                    m_holding = false;
                }
                break;
            case MouseEventType::MouseDown:
                if (m_entered) {
                    this->onMouseDown(event);
                    mouseDownPos = event->position;
                    m_holding = true;
                }
                break;
            case MouseEventType::Click:
                this->onClick();
                event->preventDefault();
        }
        int type2 = (int)type;
        if (type == MouseEventType::MouseUp && m_entered) {
            type2 = MouseEventType::Click;
        }
        else if (type == MouseEventType::Move) {
            auto bound = boundingBoxFromContentSize(this);
            bool containsCursor = bound.containsPoint(event->position);
            if (
                containsCursor
                && 
                !m_entered
            ) type2 = MouseEventType::Enter;
            if (
                !containsCursor
                && 
                m_entered
            ) type2 = MouseEventType::Exit;
        }
        if (type2 != (int)type) {
            EventTarget::dispatchEvent(new MouseEvent((MouseEventType)type2, event->position));
        }

        if (dragEnabled() && type == MouseEventType::Move && m_entered && m_holding) {
            if (CCPointExtensions::distance(
                mouseDownPos.equals(ccp(0,0)) ? event->position : mouseDownPos, 
                event->position
            ) > clickDragDistance) {
                if (!currentDragEvent) {
                    currentDragEvent = new MouseDragEvent(mouseDownPos, event->position);
                    currentDragEvent->retain();
                    onDragStart(currentDragEvent);
                } else {
                    currentDragEvent->current = event->position;
                    onDrag(currentDragEvent);
                }
            }
        }
        return;
    });
}



std::string Container::getUnitLabel(Unit unit) {
    std::string l;
    switch (unit) {
        case Unit::OpenGL: 
            l = "OpenGL Points";
            break;
        case Unit::UIKit: 
            l = "UIKit Points";
            break;
        case Unit::Percent: 
            l = "Parent's percentage";
            break;
        case Unit::Viewport: 
            l = "Viewport";
            break;
    }
    return l;
}

bool Container::init() {
    if (!CCLayerRGBA::init()) return false;
    colorBg = CCLayerColor::create(ccc4(255,255,255,255));
    colorBg->setZOrder(-999);
    colorBg->retain();
    colorBg->setAnchorPoint(ccp(0,0));
    //addChild(colorBg);

    addListener("nodeLayoutUpdate", [this](NodeEvent*j){
        onLayoutUpdate(static_cast<NodeLayoutUpdate*>(j));
        colorBg->setContentSize(getContentSize());
        checkConstraints();
    });
    InputHandlerImpl::initHandler();
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(ccp(0,0));
    updateAnchorLabel();
    updateSizeUnitLabel();
    updatePositionUnitLabel();
    //setUserObject("geode.devtools/useRealAttributes", CCBool::create(true));
    setCascadeOpacityEnabled(true);
    return true;
}

void Container::setColor(ccColor3B const& col) {
    colorBg->setColor(col);
}

bool Container::dispatchEvent(NodeEvent* event) {
    event->retain();
    auto ret = EventTarget::dispatchEvent(event);
    if (event->m_log) log::debug("[{} | EventTarget]: Dispatching {} (target: {})", getNodeName(this), event->m_eventName, getNodeName(static_cast<Container*>(event->target())));
    /*
    colorBg->setColor(ccc3(255,171,15));
    colorBg->setOpacity(150);
    colorBg->runAction(CCFadeTo::create(0.5,0));
    */
    if (!ret) return false;
    if (event->m_stopPropagate) {
        event->m_stopPropagate = false;
        // according to the parent this is not really a cancel event, but if you want 
        // you can check for m_stopPropagate like i did here
        return true;
    }
    // workaround to prevent one heck of a call stack
    //
    // TODO: use some sort of a global event listeners mapper
    // so we can just do everything in 1 call
    #define dispatch                                                                                    \
        switch (event->m_dispatchingFlow) {                                                             \
            case DispatchingFlow::Up:                                                                   \
                if (auto p = dynamic_cast<EventTarget*>(m_pParent)) return p->dispatchEvent(event);    \
            case DispatchingFlow::Down:                                                                 \
                return dispatchToChild(event);                                                          \
        }                                                                                               \
        event->release()
    if (event->eventName() == "nodeLayoutUpdate") {
        if (isRunning()) {
            dispatch;
        } else {
            if (queuedLayoutUpdate) queuedLayoutUpdate->release();
            queuedLayoutUpdate = event;
        }
    } else {
        dispatch;
    }
    return true;
}

bool Container::dispatchToChildInList(NodeEvent* event, CCArray* children) {
    event->m_dispatchingFlow = DispatchingFlow::Down;
    auto the = CCArrayExt<CCNode*>(children);

    auto rev_iter_start = std::reverse_iterator(the.end());
    auto rev_iter_end = std::reverse_iterator(the.begin());
    
    while (rev_iter_start != rev_iter_end) {
        auto node = *rev_iter_start++;
        if (auto target = typeinfo_cast<Container*>(node)) {
            if (event->m_log) log::debug("[Container]: {}",target);
            if (!target->dispatchEvent(event)) return false;
        } else {
            if (!dispatchToChildInList(event,node->getChildren())) return false;
        }
    }
    
    return true;
}

bool Container::dispatchToChild(NodeEvent* event) {
    return dispatchToChildInList(event, m_pChildren);
};

void Container::onLayoutUpdate(NodeLayoutUpdate* e) {
    if (m_pParent == nullptr) return;
    CCPoint oldP = CCNode::getPosition();
    CCPoint resP = ccp(0,0);
    auto anchor = m_anchors[m_anchor];
    auto openglPos = CCPoint(
        processUnit(m_position.x,m_positionUnit.first,true),
        processUnit(m_position.y,m_positionUnit.second,false)
    );
    // getContentWidth/Height get its values from getContentSize so like
    switch(anchor.first) {
        case ah::Left:
            resP.x = openglPos.x; 
            break;
        case ah::Center:
            resP.x = m_pParent->CCNode::getContentSize().width/2+openglPos.x; 
            break;
        case ah::Right:
            resP.x = m_pParent->CCNode::getContentSize().width-openglPos.x; 
            break;
    };
    switch(anchor.second) {
        case av::Top:
            resP.y = m_pParent->CCNode::getContentSize().height-openglPos.y; 
            break;
        case av::Center:
            resP.y = m_pParent->CCNode::getContentSize().height/2-openglPos.y; 
            break;
        case av::Bottom:
            resP.y = openglPos.y; 
            break;
    }
    //bool nothingHappens = false;
    //if (resP.equals(oldP) && !resetContentSize()) nothingHappens = true;
    //if (nothingHappens) e->stopPropagation();
    resetContentSize();
    CCNode::setPosition(resP+ccp(m_padding.t,m_padding.l));
    colorBg->setContentSize(getContentSize());
};

float Container::processUnit(float value, Unit unit, bool width) {
    if (m_pParent == nullptr && unit == Unit::Percent) {
        // does not have a parent
        return value;
    }
    switch (unit) {
    case Unit::OpenGL:
        return value;
    case Unit::UIKit:
        return value * (CCDirector::sharedDirector()->getWinSize().width / CCDirector::sharedDirector()->getOpenGLView()->getFrameSize().width);
    case Unit::Viewport:
        return value * (width ? CCDirector::sharedDirector()->getWinSize().width : CCDirector::sharedDirector()->getWinSize().height);
    case Unit::Percent:
        return (value / 100) * (width ? m_pParent->CCNode::getContentSize().width : m_pParent->CCNode::getContentSize().height);
    };
}

void Container::setSizeConstraints(CCSize const& minSize, CCSize const& maxSize) {
    // who did this
    if (minSize.width < maxSize.width && minSize.height < maxSize.height) {
        log::error("Monika: you got it the wrong way.");
        return;
    }

    minimumSize = CCSize{
        processUnit(minSize.width,m_sizeUnit.first,true),
        processUnit(minSize.height,m_sizeUnit.second,false)
    };
    maximumSize = CCSize{
        processUnit(maxSize.width,m_sizeUnit.first,true),
        processUnit(maxSize.height,m_sizeUnit.second,false)
    };
    checkConstraints();
}

void Container::checkConstraints() {
    auto currentSize = getContentSize();
    bool d = false;
    if (currentSize.width < minimumSize.width) {
        currentSize.width = minimumSize.width;
        d = true;
    }
    if (currentSize.height < minimumSize.height) {
        currentSize.height = minimumSize.height;
        d = true;
    }
    if (!maximumSize.equals(CCSize(0,0))) {
        if (currentSize.width > maximumSize.width) {
            currentSize.width = maximumSize.width;
            d = true;
        }
        if (currentSize.height > maximumSize.height) {
            currentSize.height = maximumSize.height;
            d = true;
        }
    }
    if (d) {
        // mark the node as dirty so cocos2d will actually attempt to do something with that (i suppose)
        CCNode::setContentSize(currentSize);
        dispatchToChild(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
    }
}

bool Container::resetContentSize() {
    #define acu(padx) processUnit(padx, Unit::UIKit, true)
    #define ac(pady) processUnit(pady, Unit::UIKit, false)
    auto newS = CCSize(
        processUnit(m_size.width, m_sizeUnit.first, true),
        processUnit(m_size.height,m_sizeUnit.second, false)
    );
    if (newS.equals(getContentSize())) return false;
    CCLayer::setContentSize(CCSize(
        newS.width - acu(m_padding.l) - acu(m_padding.r),
        newS.height - ac(m_padding.t) - ac(m_padding.d)
    ));
    m_sizeP = m_size;
    return true;
}
/*
bool ContainerNodeWrapper::init(CCNode* node)  {
    if (!typeinfo_cast<Container*>(node)) {
        log::error("[ContainerNodeWrapper]: The node passed does not meet the candidate to be a node: it is a {}", node);
        return false;
    }
    m_node = node;
    Container::init();
    //log::debug("[ContainerNodeWrapper]: {}", node);
    this->addChild(m_node);
    setContentSize(node->getContentSize());
    setPosition(node->getPosition());
    setAnchorPoint(node->getAnchorPoint());
    node->setPosition(ccp(0,0));
    node->setAnchorPoint(ccp(0,0));
    return true;
}

void ContainerNodeWrapper::dispatchToChild(NodeEvent* event) {
    CCObject* obj;
    log::debug("[ContainerNodeWrapper]: {} {}", this, m_node);
    auto c = m_node->getChildren();
    CCARRAY_FOREACH(c, obj) {
        if (auto node = typeinfo_cast<Container*>(obj)) {
            node->dispatchEvent(event);
        }
    }
};
*/
