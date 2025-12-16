#pragma once

#include "../shifted_box.hpp"
#include "../../painting/alignment.hpp"
#include <memory>
#include <optional>

namespace caffeine {

/// A render object that aligns its child within its constraints.
///
/// The child is positioned using the alignment to determine its position
/// within the space allocated by the parent, then shifted to account for
/// the child's own size and the parent's constraints.
class RenderAligningShiftedBox : public RenderShiftedBox {
protected:
  std::shared_ptr<AlignmentGeometry> m_alignment;

public:
  RenderAligningShiftedBox(
    std::shared_ptr<AlignmentGeometry> alignment,
    std::shared_ptr<RenderBox> child = nullptr
  ) : RenderShiftedBox(child), m_alignment(alignment) {}

  std::shared_ptr<AlignmentGeometry> getAlignment() const {
    return m_alignment;
  }

  void setAlignment(std::shared_ptr<AlignmentGeometry> alignment) {
    if (m_alignment != alignment) {
      m_alignment = alignment;
      markNeedsLayout();
    }
  }

  void alignChild() {
    std::static_pointer_cast<BoxParentData>(m_child->getParentData())->offset = m_alignment->alongSize(m_size - m_child->getSize());
  }

  virtual ~RenderAligningShiftedBox() = default;
};

/// A render object that positions its child within its constraints.
///
/// This is a specialization of [RenderAligningShiftedBox] that uses a concrete
/// [Alignment] to position its child.
class RenderPositionedBox : public RenderAligningShiftedBox {
public:
  RenderPositionedBox(
    std::shared_ptr<Alignment> alignment = nullptr,
    std::shared_ptr<RenderBox> child = nullptr,
    std::optional<float> widthFactor = std::nullopt,
    std::optional<float> heightFactor = std::nullopt
  ) : RenderAligningShiftedBox(alignment ? alignment : std::make_shared<Alignment>(0.0f, 0.0f), child),
      m_widthFactor(widthFactor),
      m_heightFactor(heightFactor) {}

  std::optional<float> getWidthFactor() const { return m_widthFactor; }
  void setWidthFactor(std::optional<float> widthFactor) {
    if (m_widthFactor != widthFactor) {
      m_widthFactor = widthFactor;
      markNeedsLayout();
    }
  }

  std::optional<float> getHeightFactor() const { return m_heightFactor; }
  void setHeightFactor(std::optional<float> heightFactor) {
    if (m_heightFactor != heightFactor) {
      m_heightFactor = heightFactor;
      markNeedsLayout();
    }
  }

  void performLayout() override;

  virtual ~RenderPositionedBox() = default;

private:
  std::optional<float> m_widthFactor;
  std::optional<float> m_heightFactor;
};

}  // namespace caffeine
