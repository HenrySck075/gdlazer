#pragma once

// we javascripting
// @note Not on osu-framework. But if you are going to argue, this is Container

#include <Geode/Geode.hpp>
#include "Event.hpp"
#include "../../../utils.hpp"
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


enum class ah {Left, Center, Right};
enum class av {Top, Center, Bottom};

std::map<Anchor, std::tuple<ah,av>> m_anchors = {
    {Anchor::TopLeft, std::make_tuple(ah::Left, av::Top)},
    {Anchor::Top, std::make_tuple(ah::Center, av::Top)},
    {Anchor::TopRight, std::make_tuple(ah::Right, av::Top)},

    {Anchor::Left, std::make_tuple(ah::Left, av::Center)},
    {Anchor::Center, std::make_tuple(ah::Center, av::Center)},
    {Anchor::Right, std::make_tuple(ah::Right, av::Center)},

    {Anchor::BottomLeft, std::make_tuple(ah::Left, av::Bottom)},
    {Anchor::Bottom, std::make_tuple(ah::Center, av::Bottom)},
    {Anchor::BottomRight, std::make_tuple(ah::Right, av::Bottom)} 
};


enum class Unit {
    /// Relative to the parent size. Value in range 0-100. Value higher or lower is allowed.
    Percent,
    /// Viewport. Value in range 0-1 with 0 is 0 and 1 is full viewport length. Value higher or lower is allowed.
    Viewport,
    /// OpenGL Point
    OpenGL,
    /// UIKit Point
    UIKit
};

/// @brief CCLayer that implements local Event system (like javascript EventTarget) + some more shit
class Container : public CCLayer {
protected:
    Anchor m_anchor = Anchor::BottomLeft;
    
    // @param value Value in specified unit
    // @param unit The unit in question
    // @returns The value in OpenGL unit
    float processUnit(float value, Unit unit, bool width) {
        switch (unit) {
            case Unit::OpenGL: return value;
            case Unit::UIKit: 
                // any value works
                return value * (CCDirector::sharedDirector()->getWinSize().width / CCDirector::sharedDirector()->getOpenGLView()->getDisplaySize().width); 
            case Unit::Viewport:
                return value * (width ? CCDirector::sharedDirector()->getWinSize().width : CCDirector::sharedDirector()->getWinSize().height);
            case Unit::Percent:
                return value / 100 * (width ? m_pParent->getContentSize().width : m_pParent->getContentSize().height);
        }
    }

private:
    using Callback = std::function<void(NodeEvent*)>;
    std::map<std::string, std::vector<Callback>> m_listeners;
    
    CCPoint m_position = CCPoint(0,0);
    // the size that is affected by m_sizeUnit
    CCSize m_size = CCSize(0,0);

    // first: hori | second: verti
    std::pair<Unit, Unit> m_sizeUnit = std::make_pair(Unit::OpenGL, Unit::OpenGL);
    virtual void onParentSizeChanged();

public:
    void addEventListener(std::string eventName, const Callback& listener);
    void removeEventListener(std::string eventName, const Callback& listener);
    virtual void dispatchEvent(NodeEvent* event);
    virtual void dispatchToChild(NodeEvent* event);

    // breaking change (not)
    void setLayout(Layout* l) {}

    bool init() override {
        auto e = CCLayer::init();
        ignoreAnchorPointForPosition(false);

        addEventListener("nodeSizeChanged", [this](NodeEvent*j){onParentSizeChanged();});
        return e;
    }

    static Container* create() {
        create_class(Container, init);
    }

    // get the j
    auto getSizeUnits() {return m_sizeUnit;}

    // set a new content size while also providing the unit that will be parsed
    void setContentSize(CCSize const& size, Unit sizeUnitHorizontal, Unit sizeUnitVertical) {
        m_sizeUnit.first = sizeUnitHorizontal;
        m_sizeUnit.second = sizeUnitVertical;
        setContentSize(size);
    };

    void setContentSize(CCSize const& size) override {
        m_size = size;
        auto unit = getSizeUnits();
        CCLayer::setContentSize(CCSize(
            processUnit(size.width, unit.first, true),
            processUnit(size.height, unit.second, false)
        ));
        dispatchToChild(new NodeUIEvent("nodeSizeChanged"));
    }
    void setContentWidth(float width) {
        CCLayer::setContentWidth(width);
        dispatchToChild(new NodeUIEvent("nodeSizeChanged"));
    }
    void setContentHeight(float height) {
        CCLayer::setContentHeight(height);
        dispatchToChild(new NodeUIEvent("nodeSizeChanged"));
    }
};

template<class T>
concept is_node = std::is_base_of_v<CCNode, T> && !std::is_base_of_v<Container, T>;

class ContainerNodeWrapper : public Container {
private:
    CCNode* m_node;
public:
    static ContainerNodeWrapper* create(CCNode* node) {
        create_class(ContainerNodeWrapper, init, node);
    }

    bool init(CCNode* node);

    void setContentSize(CCSize const& size) override {
        m_node->setContentSize(size);
        Container::setContentSize(size);
    }

    void dispatchToChild(NodeEvent* event) override;
};