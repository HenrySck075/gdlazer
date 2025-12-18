#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include <memory>
#include "../rendering/object/RenderObject.hpp"
#include "../widgets/framework/Widget.hpp"
#include "../widgets/framework/Element.hpp"

namespace caffeine {

// ============================================================================
// Column: Lays out children vertically
// ============================================================================

class RenderColumn : public ContainerRenderObjectMixin {
public:
  void performLayout() override;
  void paint(PaintingContext* context, const Offset& offset) override;
};

// ============================================================================
// Column Widget
// ============================================================================

class Column : public MultiChildRenderObjectWidget {
public:
  Column(std::vector<Widget*> children = {}) : MultiChildRenderObjectWidget(children) {}

  RefNauseam<RenderObject> createRenderObject() override;
};

}  // namespace caffeine
