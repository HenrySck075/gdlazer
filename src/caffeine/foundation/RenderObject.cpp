#include <gdlazer/caffeine/foundation/RenderObject.hpp>

// Most inline implementations are in the header
// Add any complex implementations here as needed

// TODO: Add paint implementations that actually draw to cocos2d
// TODO: Add parentData storage mechanism (could be a map or property on RenderBox)
namespace caffeine {
  void RenderObject::layout(const BoxConstraints& constraints, bool parentUsesSize ){
    m_constraints = constraints;

    m_log->debug("Constraint: {}", m_constraints);
    
    // Only layout if something changed or we're marked dirty
    if (m_needsLayout || !(m_constraints == constraints)) {
      performLayout();
      m_needsLayout = false;
    }
  }
}