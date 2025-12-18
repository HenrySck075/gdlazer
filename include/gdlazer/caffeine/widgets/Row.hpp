#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include <memory>
#include "../rendering/object/RenderObject.hpp"
#include "../widgets/framework/Widget.hpp"
#include "../widgets/framework/Element.hpp"

namespace caffeine {

// ============================================================================
// Row: Lays out children horizontally
// ============================================================================

class RenderRow : public ContainerRenderObjectMixin {
public:
  void performLayout() override;
  void paint(PaintingContext* context, const Offset& offset) override;
};

// ============================================================================
// Row Widget
// ============================================================================

class Row : public MultiChildRenderObjectWidget {
public:
  Row(std::vector<Widget*> children = {}) : MultiChildRenderObjectWidget(children) {}

  RefNauseam<RenderObject> createRenderObject() override;
};

}  // namespace caffeine
