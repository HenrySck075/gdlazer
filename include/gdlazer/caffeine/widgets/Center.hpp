#pragma once

#include <memory>
#include "../rendering/object/RenderObject.hpp"
#include "../widgets/framework/Widget.hpp"
#include "../widgets/framework/Element.hpp"
#include "gdlazer/caffeine/rendering/shifted_box.hpp"

namespace caffeine {

// ============================================================================
// Center: Centers child within available space
// ============================================================================

class RenderCenter : public RenderShiftedBox {
public:
  RenderCenter(std::shared_ptr<RenderBox> child = nullptr)
    : RenderShiftedBox(child) {}
  void performLayout() override;
};

// ============================================================================
// Center Widget
// ============================================================================

class Center : public SingleChildRenderObjectWidget {
public:
  Center(Widget* child = nullptr) : SingleChildRenderObjectWidget(child) {}

  std::shared_ptr<RenderObject> createRenderObject() override;
 };

}  // namespace caffeine
