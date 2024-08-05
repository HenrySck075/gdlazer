#include "Container.hpp"

void Container::addEventListener(std::string eventName, const Callback& listener) {
    m_listeners[eventName].push_back(listener);
};
void Container::removeEventListener(std::string eventName, const Callback& listener) {
    auto the = m_listeners[eventName];
    auto it = std::find(the.begin(),the.end(), listener);
    if (it != the.end()) {
        the.erase(it);
    }
}
void Container::dispatchEvent(NodeEvent* event) {
    auto it = m_listeners.find(event->eventName());
    if (it != m_listeners.end()) {
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

void Container::onParentSizeChanged() {
    ah h;
    av v;
    std::tie(h,v) = m_anchors[m_anchor];

    switch(h) {
        case ah::Left:
            CCLayer::setPositionX(0+m_position.x); break;
        case ah::Center:
            CCLayer::setPositionX(m_pParent->getContentWidth()/2+m_position.x); break;
        case ah::Right:
            CCLayer::setPositionX(m_pParent->getContentWidth()-m_position.x); break;
    }
    switch(v) {
        case av::Top:
            CCLayer::setPositionY(m_pParent->getContentHeight()-m_position.y); break;
        case av::Center:
            CCLayer::setPositionX(m_pParent->getContentHeight()/2-m_position.y); break;
        case av::Bottom:
            CCLayer::setPositionX(m_position.y); break;
    }
};


bool ContainerNodeWrapper::init(CCNode* node)  {
    m_node = node;
    CCLayer::setContentSize(node->getContentSize());
    CCLayer::setPosition(node->getPosition());
    CCLayer::setAnchorPoint(node->getAnchorPoint());
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