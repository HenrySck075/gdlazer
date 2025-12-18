#pragma once

#include <memory>
#include "RenderObject.hpp"
#include "../pipeline_owner.hpp"
#include "../SkiaRenderContext.hpp"
#include "../../dartui/basic_types.hpp"

namespace caffeine {

/// Root render object that orchestrates the entire rendering pipeline.
/// Mirrors Flutter's RenderView which manages the frame lifecycle:
/// - Phase 1: flushLayout() - layout all dirty nodes
/// - Phase 2: flushCompositingBits() - prepare compositing (stubbed for now)
/// - Phase 3: flushPaint() - paint all dirty nodes to canvas
/// - Phase 4: compositeFrame() - composite layers to GPU (handled via Skia)
class RenderView : public RenderBox, public RenderObjectWithChildMixin {
protected:
  std::unique_ptr<PipelineOwner> m_pipelineOwner;
  std::unique_ptr<SkiaRenderContext> m_renderContext;
  float m_width = 0;
  float m_height = 0;

public:
  RenderView();
  virtual ~RenderView() = default;

  /// Set the size of the view
  void setSize(float width, float height) {
    m_width = width;
    m_height = height;
  }

  float getWidth() const { return m_width; }
  float getHeight() const { return m_height; }

  PipelineOwner* getPipelineOwner() const { return m_pipelineOwner.get(); }
  SkiaRenderContext* getRenderContext() const { return m_renderContext.get(); }

  /// Main frame orchestration - called once per frame
  /// Follows Flutter's frame pipeline:
  void drawFrame();

  // RenderObject overrides
  void performLayout() override;
  void paint(PaintingContext* context, const Offset& offset) override;

private:
  /// Phase 1: Layout all dirty nodes
  void flushLayout();

  /// Phase 2: Prepare compositing bits (stubbed for now)
  void flushCompositingBits();

  /// Phase 3: Paint all dirty nodes
  void flushPaint();

  /// Phase 4: Composite frame (Skia snapshot handling)
  void compositeFrame();
};

}  // namespace caffeine
