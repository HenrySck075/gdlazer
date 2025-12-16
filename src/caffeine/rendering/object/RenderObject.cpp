#include <gdlazer/caffeine/rendering/object/RenderObject.hpp>

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

    geode::log::popNest();
  }
  Size ChildLayoutHelper::layoutChild(std::shared_ptr<RenderBox> child,
                                      const BoxConstraints &childConstraints) {
    if (child) {
      child->layout(childConstraints, true);
      return child->getSize();
    }
    geode::log::warn("you dont put a nullptr in layoutChild btw");
    return {0, 0};
  }
  void ChildLayoutHelper::positionChild(std::shared_ptr<RenderBox> child,
                                        const Offset &offset) {
    if (child) {
      std::static_pointer_cast<RenderBox::BoxParentData>(child->getParentData())
          ->offset = offset;
    }
  }
  } // namespace caffeine
