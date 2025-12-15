#pragma once

#include <memory>
#include "../rendering/object/RenderObject.hpp"
#include "../widgets/framework/Widget.hpp"
#include "../widgets/framework/Element.hpp"
#include "gdlazer/caffeine/rendering/shifted_box.hpp"

namespace caffeine {

// ============================================================================
// SizedBox: Fixed size container
// ============================================================================

class RenderSizedBox : public RenderShiftedBox {
private:
  Size m_size;

public:
  RenderSizedBox(Size size) : m_size(size), RenderShiftedBox(nullptr) {}

  void performLayout() override;
};

// ============================================================================
// SizedBox Widget
// ============================================================================

class SizedBox : public SingleChildRenderObjectWidget {
private:
  Size m_size;

public:
  SizedBox(Size size, Widget* child = nullptr)
    : SingleChildRenderObjectWidget(child), m_size(size) {}

  std::shared_ptr<RenderObject> createRenderObject() override;
};

}  // namespace caffeine
