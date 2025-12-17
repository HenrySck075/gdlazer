#pragma once

#include <memory>
#include "../rendering/object/RenderObject.hpp"
#include "../rendering/proxy_box/ProxyBox.hpp"
#include "framework/Widget.hpp"

namespace caffeine {

// ============================================================================
// RenderConstrainedBox
// ============================================================================

class RenderConstrainedBox : public RenderProxyBox {
protected:
  BoxConstraints m_additionalConstraints;

public:
  RenderConstrainedBox(BoxConstraints additionalConstraints = BoxConstraints(),
                       std::shared_ptr<RenderBox> child = nullptr)
    : RenderProxyBox(child), m_additionalConstraints(additionalConstraints) {}

  BoxConstraints getAdditionalConstraints() const { return m_additionalConstraints; }
  void setAdditionalConstraints(BoxConstraints constraints) {
    if (constraints == m_additionalConstraints) {
      return;
    }
    m_additionalConstraints = constraints;
    markNeedsLayout();
  }

  void performLayout() override;

  virtual ~RenderConstrainedBox() = default;
};

// ============================================================================
// ConstrainedBox Widget
// ============================================================================

class ConstrainedBox : public SingleChildRenderObjectWidget {
protected:
  BoxConstraints m_constraints;

public:
  ConstrainedBox(BoxConstraints constraints, Widget* child = nullptr)
    : SingleChildRenderObjectWidget(child), m_constraints(constraints) {}

  BoxConstraints getConstraints() const { return m_constraints; }

  std::shared_ptr<RenderObject> createRenderObject() override;
  void updateRenderObject(std::shared_ptr<BuildContext> context,
                          std::shared_ptr<RenderObject> renderObject) override;

  virtual ~ConstrainedBox() = default;
};

}  // namespace caffeine
