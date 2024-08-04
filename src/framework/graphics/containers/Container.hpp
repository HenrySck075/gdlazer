// we javascripting
// @note Not on osu-framework. But if you are going to argue, this is Container

#include <Geode/Geode.hpp>
#include "Event.hpp"

using namespace geode::prelude;

class NodeUIEvent final : public NodeEvent {
private:
    static std::string validate(std::string name) {
        if (!name.starts_with("node")) {
            throw "NodeUIEvent name must start with \"node\"";
        }
        return name;
    }
public:
    NodeUIEvent(std::string name) {
        NodeEvent(validate(name));
    };
};

/// @brief CCNode(Container) that implements local Event system (like javascript EventTarget)
class Container : public CCLayer {
private:
    using Callback = std::function<void(NodeEvent*)>;
    std::map<std::string, std::vector<Callback>> m_listeners;
public:
    void addEventListener(std::string eventName, Callback listener);
    void removeEventListener(std::string eventName, Callback listener);
    void dispatchEvent(NodeEvent* event);
    void dispatchToChild(NodeEvent* event);

    bool init() override {
        auto e = CCLayer::init();
        ignoreAnchorPointForPosition(false);
        return e;
    }
    // overrides
    void setContentSize(CCSize& const size) {
        CCLayer::setContentSize(size);
        dispatchToChild(new NodeUIEvent("nodesizechanged"));
    }
    void setContentWidth(float width) {
        CCLayer::setContentWidth(width);
        dispatchToChild(new NodeUIEvent("nodesizechanged"));
    }
    void setContentHeight(float height) {
        CCLayer::setContentHeight(height);
        dispatchToChild(new NodeUIEvent("nodesizechanged"));
    }
};