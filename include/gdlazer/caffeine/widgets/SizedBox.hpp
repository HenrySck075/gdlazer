#pragma once

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

  std::shared_ptr<RenderObject> createRenderObject() override;
  void updateRenderObject(std::shared_ptr<BuildContext>, std::shared_ptr<RenderObject> renderObject) override;
};

}  // namespace caffeine
