#pragma once

// we javascripting

#include <Geode/Geode.hpp>
#include "Event.hpp"
#include "../../input/events/MouseEvent.hpp"
#include "../../input/events/MouseDragEvent.hpp"
#include "EventTarget.hpp"
#include "../../../utils.hpp"
using namespace geode::prelude;

enum class NodeLayoutUpdateType {
    Position,
    Size,
    // specifically setParent
    All 
};

class NodeLayoutUpdate final : public NodeEvent {
public:
    NodeLayoutUpdateType type;
    NodeLayoutUpdate(NodeLayoutUpdateType t) : NodeEvent("nodeLayoutUpdate"), type(t) {
        setDispatchingFlow(DispatchingFlow::Down);
    };
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

class InputHandlerImpl : public CCLayerRGBA, public EventTarget {
    bool m_entered = false;
    bool m_holding = false;
    bool m_clickEnabled = true;
    bool m_hoverEnabled = true;

    bool m_dragEnabled = false;
    CCPoint mouseDownPos = ccp(0,0);
    // according to osu!framework
    int clickDragDistance = 10;

    MouseDragEvent* currentDragEvent = nullptr;
public:
    // for those who need to add more checks to when dragging is enabled
    virtual bool dragEnabled() {return m_dragEnabled;}
    void dragEnabled(bool enable) {m_dragEnabled = enable;}
    void initHandler();

    virtual void onMouseEnter() {};
    virtual void onMouseExit() {};
    virtual void onMouseMove(MouseEvent* event) {};
    virtual void onMouseUp(MouseEvent* event) {};
    virtual void onMouseDown(MouseEvent* event) {};
    virtual void onDragStart(MouseDragEvent* event) {};
    virtual void onDrag(MouseDragEvent* event) {};
    virtual void onDragEnd(MouseDragEvent* event) {};
    virtual void onClick() {};

    void setClickEnabled(bool e) {m_clickEnabled = e;}
    bool getClickEnabled() {return m_clickEnabled;}
    void setHoverEnabled(bool state) { 
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
    };
    bool getHoverEnabled() { return m_hoverEnabled; }
};

/// @brief CCLayer that implements some more shit
///
/// Works similar to the JavaScript event system
class Container : public InputHandlerImpl {
private:
    NodeEvent* queuedLayoutUpdate = nullptr;

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

    std::string getUnitLabel(Unit unit); 
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
    CCSize minimumSize = CCSize(0,0);
    CCSize maximumSize = CCSize(0,0);

    void checkConstraints();

    CCLayerColor* colorBg;

    bool m_includeShadow = false;

    Anchor m_anchor = Anchor::BottomLeft;
    

    virtual void onLayoutUpdate(NodeLayoutUpdate*e);
    CCPoint m_position;
    CCSize m_size = CCSize(0,0);
    CCSize m_sizeP = CCSize(0,0);

    // first: hori | second: verti
    std::pair<Unit, Unit> m_sizeUnit = std::make_pair(Unit::OpenGL, Unit::OpenGL);
    std::pair<Unit, Unit> m_positionUnit = std::make_pair(Unit::OpenGL, Unit::OpenGL);

    //bool tryDispatch(Callback& cb, NodeEvent* event) override;

    // Dispatch event to children in the given list
    // 
    // Useful when you want to dispatch event to childrens from other nodes
    bool dispatchToChildInList(NodeEvent* event, CCArray* children);

public:
    void onEnter() override {
        CCLayer::onEnter();
        if (queuedLayoutUpdate) {
            dispatchToChild(queuedLayoutUpdate);
            queuedLayoutUpdate->release();
        }
    }

    // @param value Value in specified unit
    // @param unit The unit in question
    // @returns The value in OpenGL unit
    float processUnit(float value, Unit unit, bool width);
    
    void setColor(ccColor3B const& col) override;

    bool dispatchEvent(NodeEvent* event) override;
    // Dispatches the event to the child. 
    virtual bool dispatchToChild(NodeEvent* event);

    void addChild(CCNode* e) override {
        e->setParent(nullptr);
        CCNode::addChild(e);
    }

    /// nvm
    void updateLayout() {
        log::warn("[{} | Container]: Use a FillFlowContainer instead!. I mean, it is better to split the code to multiple classes than squeezing it into one like a beginner's main.cpp, right?", getNodeName(this));
    }

    bool init();

    static Container* create() {
        create_class(Container, init);
    }
    // Sets the position anchor
    void setAnchor(Anchor anchor) {
        m_anchor = anchor;
        updateAnchorLabel();
        dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Position));
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
    /// @returns if the size is changed
    bool resetContentSize();
public:
    void setContentSize(CCSize const& size) override {
        m_size = size;
        resetContentSize();
        dispatchToChild(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
    }
    void setContentWidth(float width) {
        setContentSize(CCSize(width,getContentHeight()));
    }
    void setContentHeight(float height) {
        setContentSize(CCSize(getContentWidth(),height));
    }
    // Get the actual node content size
    const CCSize& getContentSizeWithUnit() {
        return m_size;
    }

    void setSizeConstraints(CCSize const& minSize, CCSize const& maxSize);
    std::pair<CCSize, CCSize> getSizeConstraints() {return std::make_pair(minimumSize,maximumSize);};

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
        dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Position));
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
    // Get the actual node position
    CCPoint const& getPositionWithUnit() {
        return m_position;
    }

    void setParent(CCNode* parent) override {
        CCLayer::setParent(parent);
        dispatchEvent(new NodeLayoutUpdate( NodeLayoutUpdateType::All));
    };

    ~Container() {
        colorBg->release();
    }
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
