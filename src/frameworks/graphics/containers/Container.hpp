#pragma once

#include <Geode/Geode.hpp>
#include "../../bindables/EventTarget.hpp"
#include "../../input/events/MouseEvent.hpp"
#include "../../bindables/Event.hpp"
#include "../../input/events/MouseDragEvent.hpp"

class Container;

class NodeLayoutUpdated : public Event {
public:
    explicit NodeLayoutUpdated(Container* container) 
        : Event("nodeLayoutUpdated", false), // false to prevent propagation
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

    bool init() override;

    // Mouse event virtual functions
    virtual void onMouseUp(MouseEvent* event) {}
    virtual void onMouseDown(MouseEvent* event) {}
    virtual void onMouseMove(MouseEvent* event) {}
    virtual void onMouseClick(MouseEvent* event) {}
    virtual void onMouseExit(MouseEvent* event) {}
    virtual void onMouseEnter(MouseEvent* event) {}
    virtual void onMouseDragStart(MouseDragEvent* event) {}
    virtual void onMouseDragMove(MouseDragEvent* event) {}
    virtual void onMouseDragStop(MouseDragEvent* event) {}

    // Process unit function
    float processUnit(float value, Unit unit, bool isWidth);

    // Setters for size constraints
    void setMinSize(const cocos2d::CCSize& size) { minSize = size; }
    void setMaxSize(const cocos2d::CCSize& size) { maxSize = size; }

    // Setters and getters for name
    void setName(const std::string& containerName) { name = containerName; }
    const std::string& getName() const { return name; }

    // Size setters with units
    void setSize(const cocos2d::CCSize& size, Unit unit = Unit::OpenGL) {
        setContentSize(cocos2d::CCSize(
            processUnit(size.width, unit, true),
            processUnit(size.height, unit, false)
        ));
        dispatchEvent(new NodeLayoutUpdated(this));
    }

    // Position setters with units
    void setPosition(float x, float y, Unit unit = Unit::OpenGL) {
        CCNode::setPosition(
            processUnit(x, unit, true),
            processUnit(y, unit, false)
        );
    }

    bool dispatchEvent(Event *event) override;

    void setClipChildren(bool clip);
    void setBorderRadius(float radius);

    void setBackgroundColor(const ccColor4B& color);
    const ccColor4B& getBackgroundColor() const { return m_backgroundColor; }

  protected:
    void updateClipping();
    void drawBorder();
    void draw() override;

private:
    std::string name;
    cocos2d::CCSize minSize;
    cocos2d::CCSize maxSize;
    bool m_isDragging = false;
    cocos2d::CCPoint m_lastMousePos;
    cocos2d::CCPoint m_dragStartPos;
    float m_borderRadius = 0.0f;
    cocos2d::CCDrawNode* m_borderNode = nullptr;
    cocos2d::CCDrawNode* m_backgroundNode = nullptr;
    ccColor4B m_backgroundColor = {0, 0, 0, 0};
};
