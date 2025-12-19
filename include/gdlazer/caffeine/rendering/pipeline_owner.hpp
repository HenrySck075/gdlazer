#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include "object/RenderObject.hpp"

namespace caffeine {

class PaintingContext;
struct Offset;

class PipelineOwner {
private:
  std::vector<RenderObject*> m_nodesNeedingLayout;
  std::vector<RenderObject*> m_nodesNeedingPaint;
  SkiaRenderContext* m_renderContext = nullptr;
  std::function<void()> m_onNeedVisualUpdate = nullptr;

public:
  PipelineOwner() = default;
  ~PipelineOwner() = default;

  void setRenderContext(SkiaRenderContext* context) {
    m_renderContext = context;
  }

  /// Register callback for visual update notifications (frame scheduling)
  void setOnNeedVisualUpdate(std::function<void()> callback) {
    m_onNeedVisualUpdate = callback;
  }

  /// Queue a node for layout during flushLayout()
  void addDirtyLayout(RenderObject* node) {
    if (!node) return;
    
    auto it = std::find(m_nodesNeedingLayout.begin(), m_nodesNeedingLayout.end(), node);
    if (it == m_nodesNeedingLayout.end()) {
      m_nodesNeedingLayout.push_back(node);
    }
  }

  /// Process all dirty layout nodes
  /// Sorts by depth (shallowest first) and calls layout() on each
  void flushLayout();

  /// Queue a node for painting during flushPaint()
  void addDirtyPaint(RenderObject* node) {
    if (!node) return;
    
    // Check if already in queue
    auto it = std::find(m_nodesNeedingPaint.begin(), m_nodesNeedingPaint.end(), node);
    if (it == m_nodesNeedingPaint.end()) {
      m_nodesNeedingPaint.push_back(node);
    }
  }

  /// Process all dirty paint nodes
  /// Sorts by depth (deepest first) and calls paint() on each
  void flushPaint(PaintingContext* context = nullptr, const Offset& offset = Offset{0, 0}) {
    if (m_nodesNeedingPaint.empty()) return;

    // Sort by depth: deeper nodes first (children before parents)
    std::sort(m_nodesNeedingPaint.begin(), m_nodesNeedingPaint.end(),
      [](RenderObject* a, RenderObject* b) {
        return a->getDepth() > b->getDepth();
      });

    // Swap dirty list to avoid interference during painting
    auto toRepaint = std::move(m_nodesNeedingPaint);
    m_nodesNeedingPaint.clear();

    // Paint each node with provided context
    for (auto node : toRepaint) {
      if (node->needsPaint() && context) {
        node->_paintWithContext(context, offset);
      }
    }
  }

  const std::vector<RenderObject*>& getDirtyPaintNodes() const {
    return m_nodesNeedingPaint;
  }

  void requestVisualUpdate() {
    if (m_onNeedVisualUpdate) {
      m_onNeedVisualUpdate();
    }
  }
};

}  // namespace caffeine
