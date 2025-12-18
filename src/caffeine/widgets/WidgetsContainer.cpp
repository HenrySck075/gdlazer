#include <algorithm>
#include <gdlazer/caffeine/widgets/WidgetsContainer.hpp>
#include <gdlazer/caffeine/rendering/painting_context.hpp>
#include <Geode/cocos/CCDirector.h>
#include <skia/include/core/SkImageInfo.h>
#include <vector>
#include <cstring>

namespace caffeine {

// Helper function to flip texture vertically (convert from Skia Y-up to OpenGL Y-down)
static void flipTextureVertically(uint8_t* pixelData, int width, int height) {
  const int bytesPerPixel = 4; // RGBA
  const int rowBytes = width * bytesPerPixel;

  for (int i = 0; i < height / 2; ++i) {
    uint8_t* topRow = pixelData + i * rowBytes;
    uint8_t* bottomRow = pixelData + (height - 1 - i) * rowBytes;
    
    // Swap rows
    std::swap_ranges(topRow,topRow+rowBytes,bottomRow);
  }
}

bool WidgetsContainer::init() {
  if (!CCLayer::init()) return false;

  // Create the binding object
  m_binding = std::make_unique<WidgetsFlutterBinding>();

  // Get window size and initialize layer size
  auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
  this->setContentSize(winSize);
  this->setAnchorPoint(ccp(0, 0));

  // Create the internal texture sprite
  m_textureSprite = cocos2d::CCSprite::create();
  m_textureSprite->setAnchorPoint(ccp(0, 0));
  this->addChild(m_textureSprite);

  // Initialize the widgets binding (creates build owner)
  m_binding->initWidgetsBinding();

  m_binding->addPersistentFrameCallback([this](unsigned long){paintRenderTree();});

  // Schedule this node to receive update() calls every frame
  this->scheduleUpdate();

  return true;
}

void WidgetsContainer::attachRootWidget(Widget* rootWidget) {
  if (m_binding) {
    m_binding->attachRootWidget(rootWidget);
  }
}

void WidgetsContainer::paintRenderTree() {
  if (!m_binding) return;
  
  auto rootElement = m_binding->getRootElement();
  if (!rootElement) return;

  auto rootRenderObject = rootElement->getChildRenderObject();
  if (!rootRenderObject) return;

  // Layout phase
  auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
  caffeine::BoxConstraints constraints = caffeine::BoxConstraints::tight({winSize.width, winSize.height});
  rootRenderObject->layout(constraints);

  // Begin frame with current window size
  m_skiaContext.beginFrame(static_cast<int>(winSize.width), static_cast<int>(winSize.height));
  
  // Paint phase - render to Skia canvas
  SkCanvas* canvas = m_skiaContext.getCanvas();
  if (canvas) {
    // Create painting context for the root with full bounds
    Rect bounds = Rect{0, 0, winSize.width, winSize.height};
    caffeine::PaintingContext context(canvas, bounds);
    
    // Paint the root render object tree
    rootRenderObject->paint(&context, Offset{0, 0});
  }

  // End frame and capture snapshot
  m_skiaContext.endFrame();

  // Update sprite texture from snapshot
  updateSpriteTexture();
}

void WidgetsContainer::updateSpriteTexture() {
  auto snapshot = m_skiaContext.getSnapshot();
  if (!snapshot) return;

  int width = m_skiaContext.getWidth();
  int height = m_skiaContext.getHeight();

  // Convert SkImage to pixel data
  SkImageInfo pixelInfo = SkImageInfo::Make(width, height, kRGBA_8888_SkColorType, kOpaque_SkAlphaType);
  std::vector<uint8_t> pixelData(width * height * 4);

  if (!snapshot->readPixels(pixelInfo, pixelData.data(), width * 4, 0, 0)) {
    return;
  }

  auto gomer = pixelData.data();

  // Flip the texture vertically since Skia renders with Y-up but Cocos2D expects Y-down
  flipTextureVertically(gomer, width, height);

  // Create a CCTexture2D from pixel data
  cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D();
  bool success = texture->initWithData(
    gomer,
    cocos2d::kCCTexture2DPixelFormat_RGBA8888,
    width,
    height,
    cocos2d::CCSizeMake(width, height)
  );

  if (!success) {
    texture->release();
    return;
  }
  
  // Update the internal sprite with the new texture
  auto textureContentSize = texture->getContentSize();
  m_textureSprite->setTexture(texture);
  m_textureSprite->setTextureRect(cocos2d::CCRectMake(0, 0, textureContentSize.width, textureContentSize.height));
  
  // Scale the sprite to fit the container while maintaining the aspect ratio
  auto containerSize = this->getContentSize();
  float scaleX = containerSize.width / textureContentSize.width;
  float scaleY = containerSize.height / textureContentSize.height;
  float scale = std::min(scaleX, scaleY);
  m_textureSprite->setScale(scale);
  
  texture->release();
}

void WidgetsContainer::update(float deltaTime) {
  CCLayer::update(deltaTime);

  if (!m_binding) return;

  // Frame counter for timestamp
  m_frameCount++;
  uint64_t timeStamp = m_frameCount;

  // Drive the frame pipeline
  m_binding->handleBeginFrame(timeStamp);
  
  // Transition to mid-frame microtasks phase
  m_binding->setSchedulerPhase(SchedulerPhase::midFrameMicrotasks);
  
  // Invoke persistent callbacks (triggers paintRenderTree)
  m_binding->handleDrawFrame();
}

cocos2d::CCNode* runApp(Widget* app) {
  auto container = WidgetsContainer::create();
  container->attachRootWidget(app);
  return container;
}

}
