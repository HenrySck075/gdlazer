#include <gdlazer/caffeine/rendering/pipeline_owner.hpp>
#include <algorithm>

namespace caffeine {

  // Helper function to recursively update compositing bits
  static void _updateCompositingBitsForNode(RenderObject* node) {
    if (!node) return;

    // Update this node's compositing requirements
    node->updateCompositingBits();

    // Recursively update children
    // Handle single-child nodes (RenderObjectWithChildMixin)
    auto childNode = dynamic_cast<RenderObjectWithChildMixin*>(node);
    if (childNode) {
      auto child = childNode->getChild();
      if (child) {
        _updateCompositingBitsForNode(child.get());
      }
    }

    // Handle multi-child nodes (ContainerRenderObjectMixin)
    auto containerNode = dynamic_cast<ContainerRenderObjectMixin*>(node);
    if (containerNode) {
      for (const auto& child : containerNode->getChildren()) {
        if (child) {
          _updateCompositingBitsForNode(child.get());
        }
      }
    }
  }

  void PipelineOwner::flushLayout() {
    if (m_nodesNeedingLayout.empty()) {
      return;
    }

    while (!m_nodesNeedingLayout.empty()) {
      auto toLayout = std::move(m_nodesNeedingLayout);
      m_nodesNeedingLayout.clear();

      // Sort by depth: shallowest first (parents before children)
      std::sort(toLayout.begin(), toLayout.end(),
        [](RenderObject* a, RenderObject* b) {
          return a->getDepth() < b->getDepth();
        });

      // Layout each node
      for (auto node : toLayout) {
        if (node->needsLayout() && node->getOwner() == this) {
          node->layout(node->getConstraints());
        }
      }
    }
  }

  void PipelineOwner::flushCompositingBits() {
    // Walk the render tree from root and update compositing requirements
    // This determines which nodes should be layer boundaries for optimization
    // Typically called after layout but before paint
    
    // Note: This assumes the root node is managed externally
    // In practice, RenderView will call this on itself
    // We provide the helper for tree walking
  }

}  // namespace caffeine
