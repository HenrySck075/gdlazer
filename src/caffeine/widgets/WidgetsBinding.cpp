#include "gdlazer/caffeine/widgets/WidgetsBinding.hpp"
#include "gdlazer/caffeine/widgets/RootWidget.hpp"
#include "gdlazer/caffeine/widgets/binding/BuildOwner.hpp"
#include "gdlazer/caffeine/widgets/framework/Element.hpp"

namespace caffeine {
void WidgetsBinding::attachRootWidget(Widget* rootWidget) {
  if (!rootWidget) return;

  // Wrap the user's widget in a RootWidget and create RefNauseam
  auto rootWidgetPtr = new RootWidget(
    rootWidget
  );

  // Create the root element from the widget
  m_rootElement = dynamic_cast<RootElement*>(rootWidgetPtr->createElement().get());

  // Assign the build owner to the root element (creates its BuildScope)
  m_rootElement->assignOwner(m_buildOwner);

  // Mount within build scope
  m_buildOwner->buildScope(m_rootElement.get(), [this]() {
    // Mount the root element with no parent
    m_rootElement->mount(nullptr, nullptr);
  });
}

void WidgetsBinding::drawFrame() {
  // if we dont have to refresh the frame then we simply dont bother
  // if anything, animated widgets' tickers cb on begin frame callbacks shouldve caused them to be marked as dirty
  // remove this if it breaks
  if (!m_shouldRefreshFrame) return;
  geode::log::debug("[WidgetsBinding]: attempting to rebuild");
  m_buildOwner->buildScope(m_rootElement.get());
  geode::log::debug("[WidgetsBinding]: finalizing build");
  m_buildOwner->finalizeTree();
  m_shouldRefreshFrame = false;
}

void WidgetsBinding::initWidgetsBinding() {
  m_buildOwner = std::make_shared<BuildOwner>([this](){
    geode::log::debug("[WidgetsBinding]: new dirty node yay");
    m_shouldRefreshFrame = true;
  });
}

}
