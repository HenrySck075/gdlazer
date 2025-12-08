#include <gdlazer/caffeine/foundation/ComponentElements.hpp>

void ComponentElement::performRebuild() {
  std::shared_ptr<Widget> built;
  try {
    built = build();
  } catch (std::exception&) {
    // TODO: ErrorWidget
  }
  Element::performRebuild();
  try {
    m_child = updateChild(m_child, built, m_slot);
    assert(m_child != nullptr);
  } catch (std::exception&) {
    if (m_child) m_child->deactivate();
    m_child = updateChild(nullptr, built, m_slot);
  }
};
