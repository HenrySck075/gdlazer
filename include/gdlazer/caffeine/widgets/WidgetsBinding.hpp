#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include <Geode/cocos/base_nodes/CCNode.h>
#include <memory>
#include "gdlazer/caffeine/widgets/framework/Widget.hpp"
#include "gdlazer/caffeine/widgets/RootWidget.hpp"

namespace caffeine {
/// Manages the BuildOwner and root widget mounting for the entire widget/element tree.
/// Analogous to Flutter's WidgetsBinding.
class WidgetsBinding {
protected:
  std::shared_ptr<BuildOwner> m_buildOwner;
  RefNauseam<RootElement> m_rootElement;
  bool m_shouldRefreshFrame = true;


  virtual ~WidgetsBinding() = default;

public:
  /// Returns the build owner managing this widget tree.
  BuildOwner* getBuildOwner() const { return m_buildOwner.get(); }

  /// Returns the root element.
  RootElement* getRootElement() const { return m_rootElement.get(); }

  /// Initialize the binding with an empty build owner.
  virtual void initWidgetsBinding(); 

  /// Mounts the given widget tree and creates the render tree.
  virtual void attachRootWidget(Widget* rootWidget);

  void drawFrame();
};

}
