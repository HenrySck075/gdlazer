#pragma once

// we javascripting

#include <Geode/Geode.hpp>
#include "Event.hpp"
#include "EventTarget.hpp"
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

/// @brief CCLayer that implements some more shit
///
/// Works similar to the JavaScript event system
class Container : public CCLayerRGBA, public EventTarget {
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
    CCLayerColor* colorBg;

    std::string getUnitLabel(Unit unit) {
        std::string l;
        switch (unit) {
            case Unit::OpenGL: l = "OpenGL Points";
            case Unit::UIKit: l = "UIKit Points";
            case Unit::Percent: l = "Parent's percentage";
            case Unit::Viewport: l = "Viewport";
        }
        return l;
    }
    void updateAnchorLabel() {
        setUserObject("of/anchor", CCString::create(m_anchorDebugLabel[m_anchor]));
    }
    void updateSizeUnitLabel() {
        setUserObject("of/su", CCString::create(
            "W: "+getUnitLabel(m_sizeUnit.first)+" | "
            "H: "+getUnitLabel(m_sizeUnit.second)
        ));
    }
    void updatePositionUnitLabel() {
        setUserObject("of/pu", CCString::create(
            "X: "+getUnitLabel(m_positionUnit.first)+" | "
            "Y: "+getUnitLabel(m_positionUnit.second)
        ));
    }
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
            return value * (CCDirector::sharedDirector()->getWinSize().width / CCDirector::sharedDirector()->getOpenGLView()->getFrameSize().width);
        case Unit::Viewport:
            return value * (width ? CCDirector::sharedDirector()->getWinSize().width : CCDirector::sharedDirector()->getWinSize().height);
        case Unit::Percent:
            return (value / 100) * (width ? m_pParent->CCNode::getContentSize().width : m_pParent->CCNode::getContentSize().height);
        };
    }


protected:
    virtual void onLayoutUpdate();
    CCPoint m_position = CCPoint(0,0);
    CCSize m_size = CCSize(0,0);
    CCSize m_sizeP = CCSize(0,0);

    // first: hori | second: verti
    std::pair<Unit, Unit> m_sizeUnit = std::make_pair(Unit::OpenGL, Unit::OpenGL);
    std::pair<Unit, Unit> m_positionUnit = std::make_pair(Unit::OpenGL, Unit::OpenGL);

    //bool tryDispatch(Callback& cb, NodeEvent* event) override;
    bool dispatchToChildInList(NodeEvent* event, CCArray* children);

public:
    bool dispatchEvent(NodeEvent* event) override;
    // Dispatches the event to the child. 
    virtual bool dispatchToChild(NodeEvent* event);

    void addChild(CCNode* e) override {
        e->setParent(nullptr);
        CCNode::addChild(e);
    }

    // breaking change (not)
    void setLayout(Layout* l) {}

    bool init();

    static Container* create() {
        create_class(Container, init);
    }
    // Sets the position anchor
    void setAnchor(Anchor anchor) {
        m_anchor = anchor;
        updateAnchorLabel();
        dispatchEvent(new NodeUIEvent("nodeLayoutUpdate"));
    }
    Anchor getAnchor() {
        return m_anchor;
    }
    // get the j
    auto getSizeUnit() {return m_sizeUnit;}

    // set the size unit that will be used to calculate the result size on the next `setContentSize` call
    void setSizeUnit(Unit sizeUnitHorizontal, Unit sizeUnitVertical) {
        m_sizeUnit.first = sizeUnitHorizontal;
        m_sizeUnit.second = sizeUnitVertical;
        updateSizeUnitLabel();
    }

    // set a new content size while also providing the unit that will be parsed
    void setContentSizeWithUnit(CCSize const& size, Unit sizeUnitHorizontal, Unit sizeUnitVertical) {
        setSizeUnit(sizeUnitHorizontal, sizeUnitVertical);
        setContentSize(size);
    };

private:
    void resetContentSize() {
        CCLayer::setContentSize(CCSize(
            processUnit(m_size.width, m_sizeUnit.first, true),
            processUnit(m_size.height,m_sizeUnit.second, false)
        ));
        dispatchToChild(new NodeEvent("nodeLayoutUpdate"));
        m_sizeP = m_size;
    }
public:
    void setContentSize(CCSize const& size) override {
        m_size = size;
        resetContentSize();
    }
    void setContentWidth(float width) {
        setContentSize(CCSize(width,getContentHeight()));
    }
    void setContentHeight(float height) {
        setContentSize(CCSize(getContentWidth(),height));
    }
    const CCSize& getContentSize() const override {
        return m_size;
    }
    // Get the actual node content size
    const CCSize& getRealContentSize() {
        return CCLayer::getContentSize();
    }

    // set the position unit that will be used to calculate the result position on the next `setPosition` call
    void setPositionUnit(Unit posUnitHorizontal, Unit posUnitVertical) {
        m_positionUnit.first = posUnitHorizontal;
        m_positionUnit.second = posUnitVertical;
        updatePositionUnitLabel();
    }

    void setPositionWithUnit(CCPoint const& position, Unit posUnitHorizontal, Unit posUnitVertical) {
        setPositionUnit(posUnitHorizontal, posUnitVertical);
        setPosition(position);
    }
    void setPosition(CCPoint const& position) override {
        if (position == m_position) return;
        m_position = position;
        dispatchEvent(new NodeUIEvent("nodeLayoutUpdate"));
        /*
        CCLayer::setPosition(CCPoint(
            processUnit(position.x, m_positionUnit.first, true),
            processUnit(position.y, m_positionUnit.second, false)
        ));
        */
    }
    void setPositionY(float pos) override {
        setPosition(ccp(m_position.x,pos));
    }
    void setPositionX(float pos) override {
        setPosition(ccp(pos,m_position.y));
    }
    const CCPoint& getPosition() override {
        return m_position;
    }
    float getPositionX() override {
        return m_position.x;
    }
    float getPositionY() override {
        return m_position.y;
    }
    // Get the actual node position
    CCPoint const& getRealPosition() {
        return CCLayer::getPosition();
    }

    void setParent(CCNode* parent) override {
        CCLayer::setParent(parent);
        dispatchEvent(new NodeUIEvent("nodeLayoutUpdate"));
    };
};

template<class T>
concept is_node = std::is_base_of_v<CCNode, T> && !std::is_base_of_v<Container, T>;

/*
// wraps a node inside a container
// @warning this currently does not work for some reason. So if you dont need to also resize the node, wrap it in a Container instead.
class ContainerNodeWrapper : public Container {
private:
    CCNode* m_node;
public:
    static ContainerNodeWrapper* create(CCNode* node) {
        create_class(ContainerNodeWrapper, init, node);
    }

    bool init(CCNode* node);

    void setContentSize(CCSize const& size) override {
        m_node->setContentSize(CCSize(
            processUnit(size.width, m_sizeUnit.first, true),
            processUnit(size.height, m_sizeUnit.second, false)
        ));
        Container::setContentSize(size);
    }

    CCNode* getWrappingNode() {return m_node;}

    void dispatchToChild(NodeEvent* event) override;

};
*/