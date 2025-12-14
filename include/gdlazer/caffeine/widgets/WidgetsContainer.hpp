#pragma once

#include "WidgetsBinding.hpp"
#include "SchedulerBinding.hpp"
#include "gdlazer/caffeine/foundation/SkiaRenderContext.hpp"

/// A CCSprite container that bootstraps and manages the entire widget/element tree.
/// Combines WidgetsBinding and SchedulerBinding to drive the frame pipeline.
/// Renders the widget tree using Skia to an off-screen canvas, then displays as a sprite.
class WidgetsContainer : public cocos2d::CCSprite, public WidgetsBinding, public SchedulerBinding {
public:
  CREATE_FUNC(WidgetsContainer);

  /// Mounts the given widget tree and creates the render tree.
  void attachRootWidget(Widget* rootWidget) override;

  /// Called every frame by cocos2d. Drives the frame pipeline.
  void update(float deltaTime) override;

private:
  uint64_t m_frameCount = 0;
  caffeine::SkiaRenderContext m_skiaContext;

  bool init() override;
  
  /// Paints the render object tree to the Skia canvas.
  void paintRenderTree();
  
  /// Updates the CCSprite texture from the latest Skia snapshot.
  void updateSpriteTexture();
};

/// Bootstrap the widget tree with the given root widget.
/// Returns a CCNode that contains the entire widget system.
/// The returned node should be added to the scene/parent node to render the UI.
cocos2d::CCNode* runApp(Widget* app);
