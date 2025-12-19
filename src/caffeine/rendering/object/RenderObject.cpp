#include <gdlazer/caffeine/rendering/object/RenderObject.hpp>
#include <gdlazer/caffeine/rendering/pipeline_owner.hpp>
#include <gdlazer/caffeine/rendering/painting_context.hpp>

namespace caffeine {

  void RenderObject::markNeedsLayout() {
    if (m_needsLayout) {
      return;
    }

    m_needsLayout = true;

    bool isRelayoutBoundary = m_isRelayoutBoundary.value_or(false);

    if (m_owner && isRelayoutBoundary) {
      m_owner->addDirtyLayout(this);
      m_owner->requestVisualUpdate();
    } else if (m_parent) {
      markParentNeedsLayout();
    }
  }

  void RenderObject::markParentNeedsLayout() {
    m_needsLayout = true;
    if (m_parent) {
      m_parent->markNeedsLayout();
    }
  }

  bool RenderObject::computeIsRelayoutBoundary(bool parentUsesSize) {
    bool boundary = !parentUsesSize || m_constraints.isTight() || !m_parent;
    m_isRelayoutBoundary = boundary;
    return boundary;
  }

  void RenderObject::layout(const BoxConstraints& constraints, bool parentUsesSize) {
    m_constraints = constraints;

    ///geode::log::debug("[{}]: Constraint: {}", log::getObjectName(this), m_constraints);

    geode::log::pushNest();
    
    // Compute relayout boundary status based on current constraints and parent info
    computeIsRelayoutBoundary(parentUsesSize);

    // Only layout if something changed or we're marked dirty
    if (m_needsLayout || !(m_constraints == constraints)) {
      Size oldSize = m_size;
      performLayout();
      m_needsLayout = false;
      
      // If size changed, mark paint as dirty
      if (oldSize != m_size) {
        markNeedsPaint();
      }
    }

    //geode::log::debug("[{}]: Size: {}", log::getObjectName(this), m_size);
    geode::log::popNest();
  }

  void RenderObject::markNeedsPaint() {
    log::LogNestPusher _;
    geode::log::debug("[{}::markNeedsPaint]: idk man", log::getObjectName(this));
    if (!m_needsPaint) {
      m_needsPaint = true;
      if (m_owner) {
        m_owner->addDirtyPaint(this);
        m_owner->requestVisualUpdate();
      } else if (m_parent) {
        m_parent->markNeedsPaint();
      }
    }
  }

  void RenderObject::_paintWithContext(PaintingContext* context, const Offset& offset) {
    log::LogNestPusher _;
    geode::log::debug("[{}::_paintWithContext]: {}", log::getObjectName(this), m_needsPaint);
    if (m_needsPaint) {
      paint(context, offset);
      m_needsPaint = false;
    }
  }

  Size ChildLayoutHelper::layoutChild(RefNauseam<RenderBox> child,
                                      const BoxConstraints &childConstraints) {
    if (child) {
      child->layout(childConstraints, true);
      return child->getSize();
    }
    geode::log::warn("you dont put a nullptr in layoutChild btw");
    return {0, 0};
  }
  void ChildLayoutHelper::positionChild(RefNauseam<RenderBox> child,
                                        const Offset &offset) {
    if (child) {
      static_cast<RenderBox::BoxParentData*>(child->getParentData().get())->offset = offset;
      geode::log::debug("Set <{}>'s position to {}", log::getObjectName(child.get()), offset);

    }
  }
}  // namespace caffeine
