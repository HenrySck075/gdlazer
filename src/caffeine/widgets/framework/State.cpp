#include <gdlazer/caffeine/widgets/framework/State.hpp>
#include <gdlazer/caffeine/widgets/framework/ComponentElements.hpp>

namespace caffeine {

void State::setState(std::function<void()> fn) {
  fn();
  m_element->markNeedsBuild();
}

std::shared_ptr<BuildContext> State::getContext() {
  return m_element;
};
}
