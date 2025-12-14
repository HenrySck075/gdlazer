#include <gdlazer/caffeine/widgets/WidgetsContainer.hpp>
#include <Geode/cocos/CCDirector.h>
#include <skia/include/core/SkImageInfo.h>
#include <vector>

bool WidgetsContainer::init() {
  if (!CCSprite::init()) return false;

  // Get window size and initialize sprite size
  auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
  this->setContentSize(winSize);
  this->setAnchorPoint(ccp(0, 0));

  // Initialize the widgets binding (creates build owner)
  initWidgetsBinding();

  // Schedule this node to receive update() calls every frame
  this->scheduleUpdate();

  return true;
}

void WidgetsContainer::attachRootWidget(Widget* rootWidget) {
  // Call parent implementation
  WidgetsBinding::attachRootWidget(rootWidget);
}

void WidgetsContainer::paintRenderTree() {
  // Get the root element's child render object
  auto rootElement = getRootElement();
  if (!rootElement) return;

  auto rootRenderObject = rootElement->getChildRenderObject();
  if (!rootRenderObject) return;

  // Layout phase
  auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
  caffeine::BoxConstraints constraints = caffeine::BoxConstraints::tight(winSize.width, winSize.height);
  rootRenderObject->layout(constraints);

  // Begin frame with current window size
  m_skiaContext.beginFrame(static_cast<int>(winSize.width), static_cast<int>(winSize.height));
  
  // Paint phase - render to Skia canvas
  SkCanvas* canvas = m_skiaContext.getCanvas();
  if (canvas) {
    rootRenderObject->paint(canvas);
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

  // Create a CCTexture2D from pixel data
  cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D();
  bool success = texture->initWithData(
    pixelData.data(),
    cocos2d::kCCTexture2DPixelFormat_RGBA8888,
    width,
    height,
    cocos2d::CCSizeMake(width, height)
  );

  if (!success) {
    texture->release();
    return;
  }

  // Update the sprite with the new texture
  this->setTexture(texture);
  this->setTextureRect(cocos2d::CCRectMake(0, 0, width, height));
  
  texture->release();
}

void WidgetsContainer::update(float deltaTime) {
  CCSprite::update(deltaTime);

  // Frame counter for timestamp
  m_frameCount++;
  uint64_t timeStamp = m_frameCount;

  // Drive the frame pipeline
  handleBeginFrame(timeStamp);
  
  // Transition to mid-frame microtasks phase
  m_schedulerPhase = SchedulerPhase::midFrameMicrotasks;
  
  // Invoke persistent callbacks (for future element tree rebuild hooks)
  handleDrawFrame();

  // Paint the render tree to Skia canvas and update sprite texture
  paintRenderTree();
}

cocos2d::CCNode* runApp(Widget* app) {
  auto container = WidgetsContainer::create();
  container->attachRootWidget(app);
  return container;
}
