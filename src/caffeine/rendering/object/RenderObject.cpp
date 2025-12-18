#include <gdlazer/caffeine/rendering/object/RenderObject.hpp>
#include <gdlazer/caffeine/rendering/pipeline_owner.hpp>
#include <gdlazer/caffeine/rendering/painting_context.hpp>

// Most inline implementations are in the header
// Add any complex implementations here as needed

// TODO: Add paint implementations that actually draw to cocos2d
// TODO: Add parentData storage mechanism (could be a map or property on RenderBox)
namespace caffeine {
  void RenderObject::layout(const BoxConstraints& constraints, bool parentUsesSize ){
    m_constraints = constraints;

    geode::log::debug("[{}]: Constraint: {}", log::getObjectName(this), m_constraints);

    geode::log::pushNest();
    
    // Only layout if something changed or we're marked dirty
    if (m_needsLayout || !(m_constraints == constraints)) {
      performLayout();
      m_needsLayout = false;
    }

    geode::log::debug("[{}]: Size: {}", log::getObjectName(this), m_size);
    geode::log::popNest();
  }

  void RenderObject::markNeedsPaint() {
    if (!m_needsPaint) {
      m_needsPaint = true;
      if (m_owner) {
        m_owner->addDirtyPaint(this);
      } else if (m_parent) {
        m_parent->markNeedsPaint();
      }
    }
  }

  void RenderObject::_paintWithContext(PaintingContext* context, const Offset& offset) {
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
  } // namespace caffeine
