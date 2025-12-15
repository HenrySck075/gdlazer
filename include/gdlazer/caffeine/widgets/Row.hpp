#pragma once

#include <memory>
#include "../rendering/object/RenderObject.hpp"
#include "../widgets/framework/Widget.hpp"
#include "../widgets/framework/Element.hpp"

namespace caffeine {

// ============================================================================
// Row: Lays out children horizontally
// ============================================================================

class RenderRow : public MultiChildRenderBox {
public:
  void performLayout() override;
  void paint(SkCanvas* canvas) override;
};

// ============================================================================
// Row Widget
// ============================================================================

class Row : public MultiChildRenderObjectWidget {
public:
  Row(std::vector<Widget*> children = {}) : MultiChildRenderObjectWidget(children) {}

  std::shared_ptr<RenderObject> createRenderObject() override;
};

}  // namespace caffeine
