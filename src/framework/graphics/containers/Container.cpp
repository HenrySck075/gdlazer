#include "Container.hpp"

bool Container::init() {
    auto e = CCLayer::init();
    addEventListener("nodeLayoutUpdate", [this](NodeEvent*j){onLayoutUpdate();});
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(ccp(0,0));

    return e;
}

void Container::addEventListener(std::string eventName, const Callback& listener) {
    m_listeners[eventName].push_back(listener);
};
void Container::removeEventListener(std::string eventName, const Callback& listener) {
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
    log::debug("[Container]: Dispatching event {} for {}.",event->eventName(),this);
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
    CCPoint res = ccp(0,0);
    auto anchor = m_anchors[m_anchor];
    auto openglPos = CCPoint(
        processUnit(m_position.x,m_positionUnit.first,true),
        processUnit(m_position.y,m_positionUnit.second,false)
    );
    log::debug("[Container]: OpenGL position: {}", openglPos);
    switch(anchor.first) {
        case ah::Left:
            res.x = openglPos.x; 
            break;
        case ah::Center:
            res.x = m_pParent->getContentSize().width/2+openglPos.x; 
            break;
        case ah::Right:
            res.x = m_pParent->getContentSize().width-openglPos.x; 
            break;
    };
    switch(anchor.second) {
        case av::Top:
            res.y = m_pParent->getContentHeight()-openglPos.y; 
            break;
        case av::Center:
            res.y = m_pParent->getContentHeight()/2-openglPos.y; 
            break;
        case av::Bottom:
            res.y = openglPos.y; 
            break;
    }
    //res.x = 129;
    CCNode::setPosition(res);
};


bool ContainerNodeWrapper::init(CCNode* node)  {
    m_node = node;
    m_node->retain();
    this->addChild(m_node);
    Container::init();
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