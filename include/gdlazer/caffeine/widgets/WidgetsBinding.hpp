#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include <memory>
#include "gdlazer/caffeine/foundation/Widget.hpp"
#include "gdlazer/caffeine/foundation/BuildOwner.hpp"
#include "gdlazer/caffeine/widgets/RootWidget.hpp"

/// Manages the BuildOwner and root widget mounting for the entire widget/element tree.
/// Analogous to Flutter's WidgetsBinding.
class WidgetsBinding {
protected:
  std::shared_ptr<BuildOwner> m_buildOwner;
  std::shared_ptr<RootElement> m_rootElement;

  virtual ~WidgetsBinding() = default;

public:
  /// Returns the build owner managing this widget tree.
  BuildOwner* getBuildOwner() const { return m_buildOwner.get(); }

  /// Returns the root element.
  std::shared_ptr<class RootElement> getRootElement() const { return m_rootElement; }

  /// Initialize the binding with an empty build owner.
  virtual void initWidgetsBinding() {
    m_buildOwner = std::make_shared<BuildOwner>(std::nullopt);
  }

  /// Mounts the given widget tree and creates the render tree.
  virtual void attachRootWidget(Widget* rootWidget);
};

