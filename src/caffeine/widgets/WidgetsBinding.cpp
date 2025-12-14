#include "gdlazer/caffeine/widgets/WidgetsBinding.hpp"
#include "gdlazer/caffeine/widgets/RootWidget.hpp"
#include "gdlazer/caffeine/foundation/Element.hpp"

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

