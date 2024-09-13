#include "Container.hpp"
#include "../CCPointExtensions.hpp"
#include "../../input/events/MouseEvent.hpp"

/**
 * InputHandlerImpl
 */
void Container::initHandler() {
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
                this->onClick(event);
                event->preventDefault();
        }
        int type2 = (int)type;
        if (type == MouseEventType::MouseUp && m_entered) { //&& boundingBoxFromContentSize(this).containsPoint(mouseDownPos)) {
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

void Container::setClickEnabled(bool e) { m_clickEnabled = e; }

bool Container::getClickEnabled() { return m_clickEnabled; }

void Container::setHoverEnabled(bool state) { 
    m_hoverEnabled = state; 
    if (!m_hoverEnabled) {onMouseExit();}
    else {
#ifdef GEODE_IS_WINDOWS
        auto director = CCDirector::sharedDirector();
        auto pos = director->getOpenGLView()->getMousePosition();
        auto realSize = director->getOpenGLView()->getDisplaySize();
        auto winSize = director->getWinSize();

        auto p = CCPoint(
            pos.x / realSize.width * winSize.width, 
            ((realSize.height-pos.y) / realSize.height * winSize.height)
        );

        dispatchEvent(new MouseEvent(MouseEventType::Move, p));
#endif
    }
}

bool Container::getHoverEnabled() { return m_hoverEnabled; }


/**
 * Container
 */

std::map<Anchor, std::pair<ah,av>> Container::m_anchors = {
    {Anchor::TopLeft, std::make_pair(ah::Left, av::Top)},
    {Anchor::Top, std::make_pair(ah::Center, av::Top)},
    {Anchor::TopRight, std::make_pair(ah::Right, av::Top)},

    {Anchor::Left, std::make_pair(ah::Left, av::Center)},
    {Anchor::Center, std::make_pair(ah::Center, av::Center)},
    {Anchor::Right, std::make_pair(ah::Right, av::Center)},

    {Anchor::BottomLeft, std::make_pair(ah::Left, av::Bottom)},
    {Anchor::Bottom, std::make_pair(ah::Center, av::Bottom)},
    {Anchor::BottomRight, std::make_pair(ah::Right, av::Bottom)} 
};

std::map<Anchor, std::string> Container::m_anchorDebugLabel = {
    {Anchor::TopLeft, "top left"},
    {Anchor::Top, "top center"},
    {Anchor::TopRight, "top right"},

    {Anchor::Left, "center left"},
    {Anchor::Center, "center*2"},
    {Anchor::Right, "center right"},

    {Anchor::BottomLeft, "bottom left"},
    {Anchor::Bottom, "bottom center"},
    {Anchor::BottomRight, "bottom right"} 
};

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
    if (!CCLayerColor::initWithColor(ccc4(255,255,255,0))) return false;

    addListener("nodeLayoutUpdate", [this](NodeEvent*j){
        onLayoutUpdate(static_cast<NodeLayoutUpdate*>(j));
        checkConstraints();
    });
    initHandler();
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(ccp(0,0));
    updateAnchorLabel();
    updateSizeUnitLabel();
    updatePositionUnitLabel();
    //setUserObject("geode.devtools/useRealAttributes", CCBool::create(true));
    setCascadeOpacityEnabled(true);
    return true;
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
    CCNode::setPosition(resP+CCPoint{processUnit(m_padding.t, Unit::UIKit, true),processUnit(m_padding.l, Unit::UIKit, false)});
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
    auto currentSize = CCNode::getContentSize();
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
        CCLayerColor::setContentSize(currentSize);
        // todo: remove this
        dispatchToChild(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
    }
}

bool Container::resetContentSize() {
    auto newS = CCSize(
        processUnit(m_size.width, m_sizeUnit.first, true) - processUnit(m_padding.l + m_padding.r, Unit::UIKit, true),
        processUnit(m_size.height,m_sizeUnit.second, false) - processUnit(m_padding.t + m_padding.d, Unit::UIKit, false)
    );
    if (newS.equals(CCNode::getContentSize())) return false;
    CCLayerColor::setContentSize(newS);
    // checkConstraints();
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
void Container::updateAnchorLabel() {
    setUserObject("of/anchor", CCString::create(m_anchorDebugLabel[m_anchor]));
}

void Container::updateSizeUnitLabel() {
    setUserObject("of/su",
        CCString::create(fmt::format("W: {} | H: {}", getUnitLabel(m_sizeUnit.first), getUnitLabel(m_sizeUnit.second)))
    );
}

void Container::updatePositionUnitLabel() {
  setUserObject("of/pu",
                CCString::create("X: " + getUnitLabel(m_positionUnit.first) +
                                 " | "
                                 "Y: " +
                                 getUnitLabel(m_positionUnit.second)));
}

