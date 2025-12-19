#include "gdlazer/caffeine/rendering/object/render_view.hpp"
#include "gdlazer/caffeine/rendering/painting_context.hpp"

namespace caffeine {

RenderView::RenderView() {
  m_pipelineOwner = std::make_unique<PipelineOwner>();
  m_renderContext = std::make_unique<SkiaRenderContext>();
  m_pipelineOwner->setRenderContext(m_renderContext.get());
  setOwner(m_pipelineOwner.get());
}

void RenderView::drawFrame() {
  // Phase 1: Layout
  flushLayout();

  // Phase 2: Compositing bits (prepare layer info)
  flushCompositingBits();

  // Phase 3: Paint
  flushPaint();

  // Phase 4: Composite to GPU
  compositeFrame();
}

void RenderView::flushLayout() {
  // Layout this RenderView itself with tight constraints
  BoxConstraints constraints = BoxConstraints::tight(Size(m_width, m_height));
  layout(constraints);
}

void RenderView::flushCompositingBits() {
  // Phase 2: Update compositing bits tree-wide
  // Walk the render tree and determine which nodes need to be layer boundaries
  // This allows for optimization: nodes that won't change can be cached as layers
  //
  // In Flutter, this is where:
  // - Opacity nodes are marked for layer composition
  // - Transform nodes are marked for isolated rendering
  // - Expensive subtrees are identified for caching
  //
  // For now, this is a recursive walk that lets each node determine if it
  // needs special compositing treatment (e.g., via updateCompositingBits override)
  
  if (m_child) {
    // Start tree walk from the root child
    m_child->updateCompositingBits();
  }
}

void RenderView::flushPaint() {
  // Initialize rendering context for this frame
  m_renderContext->beginFrame(static_cast<int>(m_width), static_cast<int>(m_height));

  // Get the canvas from context
  SkCanvas* canvas = m_renderContext->getCanvas();
  if (canvas) {
    // Create painting context for the root
    Rect bounds = Rect{0, 0, m_width, m_height};
    PaintingContext context(canvas, bounds);

    // Paint this RenderView and all children (depth-first tree traversal)
    // This recursively paints the entire tree, respecting parent-child offsets
    paint(&context, Offset{0, 0});
  }

  // Finalize the frame
  m_renderContext->endFrame();
}

void RenderView::compositeFrame() {
  // Phase 4: Composite frame to GPU
  // In our case, Skia has already rendered everything to the surface
  // and endFrame() has captured a snapshot.
  // The actual compositing to GPU happens via SkiaRenderContext's snapshot.
  // This is handled by WidgetsContainer when it displays the texture.
}

void RenderView::performLayout() {
  // RenderView itself doesn't have a child by default
  // Subclasses or users will attach the root widget's render object as a child
  if (m_child) {
    m_child->layout(m_constraints);
    m_size = m_child->getSize();
  } else {
    m_size = m_constraints.biggest();
  }
}

void RenderView::paint(PaintingContext* context, const Offset& offset) {
  // RenderView just delegates painting to its child
  if (m_child && context) {
    m_child->paint(context, offset);
  }
}

}  // namespace caffeine
