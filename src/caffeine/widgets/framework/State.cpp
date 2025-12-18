#include <gdlazer/caffeine/widgets/framework/State.hpp>
#include <gdlazer/caffeine/widgets/framework/ComponentElements.hpp>

namespace caffeine {

void State::setState(std::function<void()> fn) {
  fn();
  m_element->markNeedsBuild();
}

RefNauseam<BuildContext> State::getContext() {
  return dynamic_cast<BuildContext*>(m_element.get());
};
}
