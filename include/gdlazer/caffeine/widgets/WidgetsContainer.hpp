#pragma once

#include "WidgetsFlutterBinding.hpp"
#include "gdlazer/caffeine/rendering/SkiaRenderContext.hpp"

namespace caffeine {
/// A CCLayer container that bootstraps and manages the entire widget/element tree.
/// Owns a WidgetsFlutterBinding to drive the frame pipeline.
/// Renders the widget tree using Skia to an off-screen canvas, then displays via an internal CCSprite.
class WidgetsContainer : public cocos2d::CCLayer {
public:
  CREATE_FUNC(WidgetsContainer);

  /// Mounts the given widget tree and creates the render tree.
  void attachRootWidget(Widget* rootWidget);

  /// Called every frame by cocos2d. Drives the frame pipeline.
  void update(float deltaTime) override;

private:
  std::unique_ptr<WidgetsFlutterBinding> m_binding;
  uint64_t m_frameCount = 0;
  caffeine::SkiaRenderContext m_skiaContext;
  cocos2d::CCSprite* m_textureSprite = nullptr;

  bool init() override;
  
  /// Paints the render object tree to the Skia canvas.
  void paintRenderTree();
  
  /// Updates the internal sprite texture from the latest Skia snapshot.
  void updateSpriteTexture();
};

/// Bootstrap the widget tree with the given root widget.
/// Returns a CCNode that contains the entire widget system.
/// The returned node should be added to the scene/parent node to render the UI.
cocos2d::CCNode* runApp(Widget* app);

}
