#pragma once

#include <memory>
#include "../rendering/object/RenderObject.hpp"
#include "../widgets/framework/Widget.hpp"
#include "../widgets/framework/Element.hpp"

namespace caffeine {

// ============================================================================
// Column: Lays out children vertically
// ============================================================================

class RenderColumn : public MultiChildRenderBox {
public:
  void performLayout() override;
  void paint(SkCanvas* canvas) override;
};

// ============================================================================
// Column Widget
// ============================================================================

class Column : public MultiChildRenderObjectWidget {
public:
  Column(std::vector<Widget*> children = {}) : MultiChildRenderObjectWidget(children) {}

  std::shared_ptr<RenderObject> createRenderObject() override;
};

}  // namespace caffeine
