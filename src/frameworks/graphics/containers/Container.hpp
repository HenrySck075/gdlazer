#pragma once

#include <Geode/Geode.hpp>
#include "../../bindables/EventTarget.hpp"
#include "../../input/events/MouseEvent.hpp"
#include "../../bindables/Event.hpp"
#include "../../input/events/MouseDragEvent.hpp"

#include "../../macro.h"

GDL_NS_START

class Container;

class NodeLayoutUpdated : public Event {
public:
    explicit NodeLayoutUpdated(Container* container) 
        : Event("nodeLayoutUpdated"), // false to prevent propagation
          m_container(container) {}

    Container* getContainer() const { return m_container; }

private:
    Container* m_container;
};


class Container : public cocos2d::CCClippingNode, public EventTarget {
public:
    enum class Unit {
        Percent,
        Viewport,
        OpenGL,
        UIKit
    };

    static Container* create() {
      Container* ret = new Container();
      if (ret && ret->init()) {
        ret->autorelease();
        return ret;
      } else {
        delete ret;
        return nullptr;
      }
    }
    void visit() override {
      if (!m_clippingEnabled) CCNode::visit();
      else CCClippingNode::visit();
    }
    
    bool init() override;

    // Mouse events
    inline bool getTouchEnabled() {return m_touchEnabled;};
    inline void setTouchEnabled(bool e) {m_touchEnabled = e;};
    virtual void onMouseUp(MouseEvent* event) {}
    virtual void onMouseDown(MouseEvent* event) {}
    virtual void onMouseMove(MouseEvent* event) {}
    virtual void onMouseClick(MouseEvent* event) {}
    virtual void onMouseExit(MouseEvent* event) {}
    virtual void onMouseEnter(MouseEvent* event) {}
    virtual void onMouseDragStart(MouseDragEvent* event) {}
    virtual void onMouseDragMove(MouseDragEvent* event) {}
    virtual void onMouseDragStop(MouseDragEvent* event) {}

    // General-use unit conversion function
    float processUnit(float value, Unit unit, bool isWidth);

    // Constraints
    void setMinSize(const cocos2d::CCSize &size);
    void setMaxSize(const cocos2d::CCSize &size);

    // Setters and getters for name
    void setName(const std::string& containerName) { m_name = containerName; }
    const std::string& getName() const { return m_name; }

    // Size setters with units
    void setSize(const cocos2d::CCSize &size, Unit unit = Unit::OpenGL);

    // Position setters with units
    void setPosition(cocos2d::CCPoint position, Unit unit = Unit::OpenGL);

    void setParent(cocos2d::CCNode *parent) override;

    bool dispatchEvent(Event *event) override;

    void setBorderRadius(float radius);

    void setBackgroundColor(const ccColor4B& color);
    const ccColor4B& getBackgroundColor() const { return m_backgroundColor; }

    bool getClippingEnabled() const { return m_clippingEnabled; }
    void setClippingEnabled(bool enabled);

  protected:
    void updateClipping();
    void drawBorder();
    void draw() override;
    virtual void updateSizeWithUnit();
    void updatePositionWithUnit();

private:
    bool m_touchEnabled = false;
    std::string m_name;
    cocos2d::CCSize m_minSize;
    cocos2d::CCSize m_maxSize;
    bool m_isDragging = false;
    cocos2d::CCPoint m_lastMousePos;
    cocos2d::CCPoint m_dragStartPos;
    float m_borderRadius = 0.0f;
    cocos2d::CCDrawNode* m_borderNode = nullptr;
    cocos2d::CCDrawNode* m_backgroundNode = nullptr;
    ccColor4B m_backgroundColor = {0, 0, 0, 0};
    cocos2d::CCSize m_size;
    Unit m_lastSizeUnit = Unit::OpenGL;
    cocos2d::CCPoint m_positionA;
    Unit m_lastPositionUnit = Unit::OpenGL;
    bool m_clippingEnabled = true;
};

GDL_NS_END