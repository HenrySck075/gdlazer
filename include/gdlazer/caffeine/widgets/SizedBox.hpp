#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include <memory>
#include "../rendering/object/RenderObject.hpp"
#include "../widgets/framework/Widget.hpp"
#include "../widgets/framework/Element.hpp"

namespace caffeine {

// ============================================================================
// SizedBox Widget
// ============================================================================

class SizedBox : public SingleChildRenderObjectWidget {
private:
  BoxConstraints m_additionalConstraints;

public:
  SizedBox(Size size, Widget* child = nullptr)
    : SingleChildRenderObjectWidget(child), m_additionalConstraints(BoxConstraints::tight(size)) {}

  RefNauseam<RenderObject> createRenderObject() override;
  void updateRenderObject(RefNauseam<BuildContext>, RefNauseam<RenderObject> renderObject) override;
};

}  // namespace caffeine
