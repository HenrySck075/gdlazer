#pragma once


#include "gdlazer/caffeine/rendering/object/RenderObject.hpp"
namespace caffeine {
  class RenderShiftedBox : public RenderBox, public RenderObjectWithChildMixin {
  public:
    RenderShiftedBox(std::shared_ptr<RenderBox> child) {m_child = child;};
    void paint(SkCanvas* canvas) override {
      if (m_child) {
        auto offset = std::static_pointer_cast<BoxParentData>(m_child->getParentData())->offset;
        canvas->save();
        canvas->translate(offset.dx, offset.dy);
        m_child->paint(canvas);
        canvas->restore();
      }
    };

    // Position helper: call this after getting child size
    inline void positionChild(const Offset& offset) {
      ChildLayoutHelper::positionChild(std::dynamic_pointer_cast<RenderBox>(m_child), offset);
    }
  };
}
