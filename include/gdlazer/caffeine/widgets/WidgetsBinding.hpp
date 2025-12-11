#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include <memory>
#include "gdlazer/caffeine/foundation/Widget.hpp"
#include "gdlazer/caffeine/foundation/BuildOwner.hpp"
#include "SchedulerBinding.hpp"

/// A CCNode container that bootstraps and manages the entire widget/element tree.
/// Combines WidgetsBinding and SchedulerBinding to drive the frame pipeline.
/// This node hosts the root element's render tree as its sole child.
class WidgetsContainer : public cocos2d::CCLayer, public SchedulerBinding {
public:
  CREATE_FUNC(WidgetsContainer);

  /// Returns the build owner managing this widget tree.
  BuildOwner* getBuildOwner() const { return m_buildOwner.get(); }

  /// Mounts the given widget tree and creates the render tree.
  /// Called internally by runApp.
  void attachRootWidget(Widget* rootWidget);

  /// Called every frame by cocos2d. Drives the frame pipeline.
  void update(float deltaTime) override;

private:
  std::shared_ptr<BuildOwner> m_buildOwner;
  std::shared_ptr<class RootElement> m_rootElement;
  uint64_t m_frameCount = 0;

  bool init() override;
};

/// Bootstrap the widget tree with the given root widget.
/// Returns a CCNode that contains the entire widget system.
/// The returned node should be added to the scene/parent node to render the UI.
cocos2d::CCNode* runApp(Widget* app);
