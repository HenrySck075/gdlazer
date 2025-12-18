#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include <memory>
#include "../painting/alignment.hpp"
#include "../rendering/proxy_box/aligned_box.hpp"
#include "framework/Widget.hpp"

namespace caffeine {

// ============================================================================
// Align Widget
// ============================================================================

class Align : public SingleChildRenderObjectWidget {
protected:
  std::shared_ptr<AlignmentGeometry> m_alignment;
  float m_widthFactor;
  float m_heightFactor;

public:
  Align(
    std::shared_ptr<AlignmentGeometry> alignment,
    Widget* child = nullptr,
    float widthFactor = 1.0f,
    float heightFactor = 1.0f
  ) : SingleChildRenderObjectWidget(child),
      m_alignment(alignment),
      m_widthFactor(widthFactor),
      m_heightFactor(heightFactor) {}

  std::shared_ptr<AlignmentGeometry> getAlignment() const { return m_alignment; }
  float getWidthFactor() const { return m_widthFactor; }
  float getHeightFactor() const { return m_heightFactor; }

  RefNauseam<RenderObject> createRenderObject() override;
  void updateRenderObject(RefNauseam<BuildContext> context, 
                          RefNauseam<RenderObject> renderObject) override;

  virtual ~Align() = default;
};

}  // namespace caffeine
