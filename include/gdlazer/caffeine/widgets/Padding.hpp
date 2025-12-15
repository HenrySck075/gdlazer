#pragma once

#include <memory>
#include "../rendering/object/RenderObject.hpp"
#include "framework/Widget.hpp"
#include "framework/Element.hpp"
#include "../painting/edge_insets.hpp"
#include "gdlazer/caffeine/rendering/shifted_box.hpp"

namespace caffeine {

// ============================================================================
// Padding: Adds padding around child
// ============================================================================

class RenderPadding : public RenderShiftedBox {
private:
  EdgeInsets m_insets;

public:
  RenderPadding(EdgeInsets insets, std::shared_ptr<RenderBox> child)
    : m_insets(insets), RenderShiftedBox(child) {}

  void performLayout() override;
};

// ============================================================================
// Padding Widget
// ============================================================================

class Padding : public SingleChildRenderObjectWidget {
private:
  EdgeInsets m_insets;

public:
  Padding(EdgeInsets insets, Widget* child = nullptr)
    : SingleChildRenderObjectWidget(child), m_insets(insets) {}

  inline EdgeInsets getInsets() {return m_insets;}

  std::shared_ptr<RenderObject> createRenderObject() override;
};

}  // namespace caffeine
