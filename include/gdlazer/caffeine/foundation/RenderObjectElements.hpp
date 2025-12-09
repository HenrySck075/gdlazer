#pragma once

#include "Element.hpp"
#include "Widget.hpp"
#include <vector>

/* An Element that renders a CCNode (RenderObject) directly.
 *
 * Each RenderObjectWidget has a corresponding RenderObjectElement.
 * This is the base class for elements that manage render objects.
 */

class SingleChildRenderObjectElement : public RenderObjectElement {
protected:
  std::shared_ptr<Element> m_child;
  std::shared_ptr<SingleChildRenderObjectWidget> m_widget;

public:
  SingleChildRenderObjectElement(SingleChildRenderObjectWidget* widget)
    : RenderObjectElement(widget), m_widget(std::shared_ptr<SingleChildRenderObjectWidget>(widget)) {}

  void performRebuild() override;
  void update(Widget* newWidget) override;
  std::shared_ptr<Element> getAttachingRenderObjectChild() override;
  void visitChildren(std::function<void(std::shared_ptr<Element>)> visitor) override;

  cocos2d::CCNode* createRenderObject() override;
  virtual void insertRenderObjectChild(cocos2d::CCNode* child) override {}
  virtual void removeRenderObjectChild(cocos2d::CCNode* child) override {}
};


class MultiChildRenderObjectElement : public RenderObjectElement {
protected:
  std::vector<std::shared_ptr<Element>> m_children;
  std::shared_ptr<MultiChildRenderObjectWidget> m_widget;

public:
  MultiChildRenderObjectElement(MultiChildRenderObjectWidget* widget)
    : RenderObjectElement(widget), m_widget(std::shared_ptr<MultiChildRenderObjectWidget>(widget)) {}

  void performRebuild() override;
  void update(Widget* newWidget) override;
  void visitChildren(std::function<void(std::shared_ptr<Element>)> visitor) override;

  cocos2d::CCNode* createRenderObject() override;
  virtual void insertRenderObjectChild(cocos2d::CCNode* child) override;
  virtual void removeRenderObjectChild(cocos2d::CCNode* child) override;
};
