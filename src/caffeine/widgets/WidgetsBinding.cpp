#include "gdlazer/caffeine/widgets/WidgetsBinding.hpp"
#include "gdlazer/caffeine/widgets/RootWidget.hpp"
#include "gdlazer/caffeine/widgets/binding/BuildOwner.hpp"
#include "gdlazer/caffeine/widgets/framework/Element.hpp"

namespace caffeine {
void WidgetsBinding::attachRootWidget(Widget* rootWidget) {
  if (!rootWidget) return;

  // Wrap the user's widget in a RootWidget and create shared_ptr
  auto rootWidgetPtr = std::make_shared<RootWidget>(rootWidget);

  // Create the root element from the widget
  m_rootElement = std::static_pointer_cast<RootElement>(rootWidgetPtr->createElement());

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
  m_buildOwner->buildScope(m_rootElement.get());
  m_buildOwner->finalizeTree();
}

void WidgetsBinding::initWidgetsBinding() {
  m_buildOwner = std::make_shared<BuildOwner>([this](){m_shouldRefreshFrame = true;});
}

}
