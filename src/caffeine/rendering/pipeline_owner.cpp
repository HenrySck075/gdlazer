#include <gdlazer/caffeine/rendering/pipeline_owner.hpp>
#include <algorithm>

namespace caffeine {

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

}  // namespace caffeine
