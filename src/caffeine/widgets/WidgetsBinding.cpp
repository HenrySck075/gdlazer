#include "gdlazer/caffeine/widgets/WidgetsBinding.hpp"
#include "gdlazer/caffeine/widgets/RootWidget.hpp"
#include "gdlazer/caffeine/foundation/Element.hpp"
#include "gdlazer/caffeine/foundation/BuildScope.hpp"

bool WidgetsContainer::init() {
  if (!CCNode::init()) return false;

  // Create build owner with empty callback for now
  m_buildOwner = std::make_unique<BuildOwner>(std::nullopt);

  return true;
}

void WidgetsContainer::attachRootWidget(Widget* rootWidget) {
  if (!rootWidget) return;

  // Wrap the user's widget in a RootWidget and create shared_ptr
  auto rootWidgetPtr = std::make_shared<RootWidget>(rootWidget);

  // Create the root element from the widget
  m_rootElement = std::static_pointer_cast<RootElement>(rootWidgetPtr->createElement());

  // Mount within build scope
  m_buildOwner->buildScope(m_rootElement.get(), [this]() {
    // Mount the root element with no parent
    m_rootElement->mount(nullptr, nullptr);
  });

  // Get the render object (CCNode) from the root element's child
  // and add it as a child of this container
  if (m_rootElement) {
    if (auto childElement = m_rootElement->getChild()) {
      if (auto renderObj = childElement->getRenderObject()) {
        this->addChild(renderObj);
      }
    }
  }
}

cocos2d::CCNode* runApp(Widget* app) {
  auto container = WidgetsContainer::create();
  container->attachRootWidget(app);
  return container;
}
