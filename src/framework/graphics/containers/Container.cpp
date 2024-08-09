#include "Container.hpp"

bool Container::init() {
    auto e = CCLayer::init();
    addListener("nodeLayoutUpdate", [this](NodeEvent*j){onLayoutUpdate();});
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(ccp(0,0));
    setUserObject("geode.devtools/useRealAttributes", CCBool::create(true));

    return e;
}

void Container::addListener(std::string eventName, const Callback& listener) {
    m_listeners[eventName].push_back(listener);
};
void Container::removeListener(std::string eventName, const Callback& listener) {
    /*
    auto the = m_listeners[eventName];
    auto it = std::find(the.begin(),the.end(), listener);
    if (it != the.end()) {
        the.erase(it);
    }
    */
}
void Container::dispatchEvent(NodeEvent* event) {
    auto it = m_listeners.find(event->eventName());
    if (it == m_listeners.end()) {
        log::warn("[Container]: Event {} not in list.",event->eventName());
        return;
    }
    for (auto i : m_listeners[event->eventName()]) {
        i(event);
    }
};

void Container::dispatchToChild(NodeEvent* event) {
    CCObject* obj;
    CCARRAY_FOREACH(m_pChildren, obj) {
        if (auto node = dynamic_cast<Container*>(obj)) {
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
    switch(anchor.first) {
        case ah::Left:
            resP.x = openglPos.x; 
            break;
        case ah::Center:
            resP.x = m_pParent->getContentWidth()/2+openglPos.x; 
            break;
        case ah::Right:
            resP.x = m_pParent->getContentWidth()-openglPos.x; 
            break;
    };
    switch(anchor.second) {
        case av::Top:
            resP.y = m_pParent->getContentHeight()-openglPos.y; 
            break;
        case av::Center:
            resP.y = m_pParent->getContentHeight()/2-openglPos.y; 
            break;
        case av::Bottom:
            resP.y = openglPos.y; 
            break;
    }
    CCNode::setPosition(resP);
    resetContentSize();
};

bool ContainerNodeWrapper::init(CCNode* node)  {
    if (!dynamic_cast<Container*>(node)) {
        log::error("[ContainerNodeWrapper]: The node passed does not meet the candidate to be a node: it is a {}", node);
        return false;
    }
    Container::init();
    m_node = node;
    m_node->retain();
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
    CCARRAY_FOREACH(m_node->getChildren(), obj) {
        if (auto node = dynamic_cast<Container*>(obj)) {
            node->dispatchEvent(event);
        }
    }
};