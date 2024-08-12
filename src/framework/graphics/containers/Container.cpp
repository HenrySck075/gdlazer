#include "Container.hpp"

bool Container::init() {
    auto e = CCLayer::init();
    addListener("nodeLayoutUpdate", [this](NodeEvent*j){onLayoutUpdate();});
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(ccp(0,0));
    //setUserObject("geode.devtools/useRealAttributes", CCBool::create(true));

    return e;
}

void Container::dispatchEventUnsafe(NodeEvent* event) {
    for (auto i : m_listeners[event->eventName()]) {
        if (tryDispatch(i,event)) break;
    }
    if (event->m_stopPropagate) return;
    switch (event->m_dispatchingFlow) {
      case DispatchingFlow::Up:
        if (auto p = typeinfo_cast<Container*>(m_pParent)) p->dispatchEvent(event);
      case DispatchingFlow::Down:
        dispatchToChild(event);
    }
}

void Container::dispatchToChild(NodeEvent* event) {
    CCObject* obj;
    CCARRAY_FOREACH(m_pChildren, obj) {
        if (auto node = typeinfo_cast<Container*>(obj)) {
            node->dispatchEvent(event);
        }
    }
};

void Container::onLayoutUpdate() {
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
};

bool ContainerNodeWrapper::init(CCNode* node)  {
    /*
    if (!typeinfo_cast<Container*>(node)) {
        log::error("[ContainerNodeWrapper]: The node passed does not meet the candidate to be a node: it is a {}", node);
        return false;
    }
    */
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
