#pragma once


#include "gdlazer/caffeine/rendering/object/RenderObject.hpp"
#include "gdlazer/caffeine/foundation/utils/Ref.hpp"
namespace caffeine {
  class RenderShiftedBox : public RenderBox, public RenderObjectWithChildMixin {
  public:
    RenderShiftedBox(RefNauseam<RenderBox> child) {m_child = child;};
    void paint(PaintingContext* context, const Offset& offset) override {
      if (m_child) {
        auto offset_data = static_cast<BoxParentData*>(m_child->getParentData().get())->offset;
        // Note: Will need to implement canvas translation in PaintingContext
        m_child->paint(context, offset_data);
      }
    };

    // Position helper: call this after getting child size
    inline void positionChild(const Offset& offset) {
      ChildLayoutHelper::positionChild(dynamic_cast<RenderBox*>(m_child.get()), offset);
    }
  };
}
