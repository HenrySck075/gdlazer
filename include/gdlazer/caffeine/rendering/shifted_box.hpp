#pragma once


#include "gdlazer/caffeine/rendering/object/RenderObject.hpp"
namespace caffeine {
  class RenderShiftedBox : public SingleChildRenderBox {
  public:
    RenderShiftedBox(std::shared_ptr<RenderBox> child)
      : SingleChildRenderBox(child) {};
    void paint(SkCanvas* canvas) override {
      if (m_child) {
        auto offset = m_child->getParentData()->offset;
        canvas->save();
        canvas->translate(offset.dx, offset.dy);
        m_child->paint(canvas);
        canvas->restore();
      }
    };
  };
}
