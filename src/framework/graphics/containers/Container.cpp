#include "Container.hpp"

bool Container::init() {
    if (!CCLayerRGBA::init()) return false;
    colorBg = CCLayerColor::create(ccc4(255,255,255,0));
    colorBg->setZOrder(-999);
    colorBg->retain();
    colorBg->setAnchorPoint(ccp(0,0));
    //addChild(colorBg);
    addListener("nodeLayoutUpdate", [this](NodeEvent*j){
        onLayoutUpdate(static_cast<NodeUIEvent*>(j));
        colorBg->setContentSize(getRealContentSize());
    });
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(ccp(0,0));
    updateAnchorLabel();
    updateSizeUnitLabel();
    updatePositionUnitLabel();
    //setUserObject("geode.devtools/useRealAttributes", CCBool::create(true));

    return true;
}

void Container::setColor(ccColor3B const& col) {
    colorBg->setColor(col);
}
void Container::setOpacity(GLubyte opacity) {
    colorBg->setOpacity(opacity);
}

bool Container::dispatchEvent(NodeEvent* event) {
    if (event->m_log) log::debug("[{} | Container]: Dispatching {}", getNodeName(this), event->m_eventName);
    auto ret = EventTarget::dispatchEvent(event);
    /*
    colorBg->setColor(ccc3(255,171,15));
    colorBg->setOpacity(150);
    colorBg->runAction(CCFadeTo::create(0.5,0));
    */
    if (!ret) return false;
    if (event->m_stopPropagate) {
        event->m_stopPropagate = true;
        return false;
    }
    switch (event->m_dispatchingFlow) {
      case DispatchingFlow::Up:
        if (auto p = typeinfo_cast<Container*>(m_pParent)) return p->dispatchEvent(event);
      case DispatchingFlow::Down:
        return dispatchToChild(event);
    }
    return true;
}

bool Container::dispatchToChildInList(NodeEvent* event, CCArray* children) {
    event->m_dispatchingFlow = DispatchingFlow::Down;
    CCObject* obj;
    CCARRAY_FOREACH_REVERSE(children, obj) {
        if (auto node = typeinfo_cast<Container*>(obj)) {
            if (!node->dispatchEvent(event)) return false;
        } else {
            if (!dispatchToChildInList(event,typeinfo_cast<CCNode*>(obj)->getChildren())) return false;
        }
    }
    return true;
}

bool Container::dispatchToChild(NodeEvent* event) {
    return dispatchToChildInList(event, m_pChildren);
};

void Container::onLayoutUpdate(NodeUIEvent* e) {
    
    if (m_pParent == nullptr) return;
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
    CCNode::setPosition(resP);
    resetContentSize();
    colorBg->setContentSize(getRealContentSize());
};

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