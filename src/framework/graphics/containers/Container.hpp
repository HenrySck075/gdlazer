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
    NodeUIEvent(std::string name) : NodeEvent(validate(name)) {};
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
private:
    enum class ah {Left, Center, Right};
    enum class av {Top, Center, Bottom};

    std::map<Anchor, std::pair<ah,av>> m_anchors = {
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
    std::map<Anchor, std::string> m_anchorDebugLabel = {
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
protected:
    Anchor m_anchor = Anchor::BottomLeft;
    
    // @param value Value in specified unit
    // @param unit The unit in question
    // @returns The value in OpenGL unit
    float processUnit(float value, Unit unit, bool width) {
        if (m_pParent == nullptr && unit == Unit::Percent) {
            // does not have a parent
            return value;
        }
        switch (unit) {
            case Unit::OpenGL: 
                return value;
            case Unit::UIKit: 
                return value * (CCDirector::sharedDirector()->getWinSize().width / CCDirector::sharedDirector()->getOpenGLView()->getDisplaySize().width); 
            case Unit::Viewport:
                return value * (width ? CCDirector::sharedDirector()->getWinSize().width : CCDirector::sharedDirector()->getWinSize().height);
            case Unit::Percent:
                return value / 100 * (width ? m_pParent->getContentWidth() : m_pParent->getContentHeight());
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
    std::pair<Unit, Unit> m_positionUnit = std::make_pair(Unit::OpenGL, Unit::OpenGL);
    virtual void onParentSizeChanged();

public:
    void addEventListener(std::string eventName, const Callback& listener);
    void removeEventListener(std::string eventName, const Callback& listener);
    virtual void dispatchEvent(NodeEvent* event);
    virtual void dispatchToChild(NodeEvent* event);

    // breaking change (not)
    void setLayout(Layout* l) {}

    bool init() override;

    static Container* create() {
        create_class(Container, init);
    }

    // get the j
    auto getSizeUnits() {return m_sizeUnit;}

    // set a new content size while also providing the unit that will be parsed
    void setContentSizeWithUnit(CCSize const& size, Unit sizeUnitHorizontal, Unit sizeUnitVertical) {
        m_sizeUnit.first = sizeUnitHorizontal;
        m_sizeUnit.second = sizeUnitVertical;
        setContentSize(size);
    };

    void setContentSize(CCSize const& size) override {
        m_size = size;
        auto unit = getSizeUnits();
        CCLayer::setContentSize(CCSize(
            processUnit(size.width, m_sizeUnit.first, true),
            processUnit(size.height,m_sizeUnit.second, false)
        ));
        dispatchToChild(new NodeUIEvent("nodeLayoutUpdate"));
    }
    void setContentWidth(float width) {
        CCLayer::setContentWidth(width);
        dispatchToChild(new NodeUIEvent("nodeLayoutUpdate"));
    }
    void setContentHeight(float height) {
        CCLayer::setContentHeight(height);
        dispatchToChild(new NodeUIEvent("nodeLayoutUpdate"));
    }

    void setPositionWithUnit(CCPoint const& position, Unit posUnitHorizontal, Unit posUnitVertical) {
        m_positionUnit.first = posUnitHorizontal;
        m_positionUnit.second = posUnitVertical;
        setPosition(position);
        dispatchEvent(new NodeUIEvent("nodeLayoutUpdated"));
    }
    void setPosition(CCPoint const& position) override {
        m_position = position;
        CCLayer::setPosition(CCPoint(
            processUnit(position.x, m_positionUnit.first, true),
            processUnit(position.y, m_positionUnit.second, false)
        ));
    }

    void setParent(CCNode* parent) override {
        CCLayer::setParent(parent);
        log::debug("[Container]: im not fine.");
        dispatchEvent(new NodeUIEvent("nodeLayoutUpdate"));
        setContentSizeWithUnit(m_size,m_sizeUnit.first,m_sizeUnit.second);
    };
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
        Container::setContentSize(size);
        m_node->setContentSize(getContentSize());
    }

    void dispatchToChild(NodeEvent* event) override;

    ~ContainerNodeWrapper() {
        m_node->release();
        Container::~Container();
    }
};