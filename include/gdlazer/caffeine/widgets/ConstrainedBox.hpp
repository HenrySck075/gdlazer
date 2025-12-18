#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include <memory>
#include "../rendering/object/RenderObject.hpp"
#include "framework/Widget.hpp"

namespace caffeine {

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

  RefNauseam<RenderObject> createRenderObject() override;
  void updateRenderObject(RefNauseam<BuildContext> context,
                          RefNauseam<RenderObject> renderObject) override;

  virtual ~ConstrainedBox() = default;
};

}  // namespace caffeine
