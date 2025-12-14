# Rendering Pipeline Implementation Plan (Skia Edition)

**Date:** 2025-12-13
**Status:** ✅ COMPLETE - Skia Integration Implemented
**Complexity:** HIGH - Major architectural shift

## Objective
Replace broken CCDrawNode with Skia for rendering RenderObjects. Use WidgetsContainer as a CCSprite to display rendered Skia canvas.

## Rationale
1. **CCDrawNode is broken** - Not reliable for production use
2. **Clipping support needed** - Skia provides proper clip functionality for future widgets
3. **Flutter uses Skia** - Proven architecture, better performance
4. **Canvas API** - Clean abstraction between RenderObject paint logic and backend

## Architecture Overview

### Rendering Stack (Bottom to Top):
```
RenderObject.paint(SkCanvas*)
    ↓
  Skia Canvas (in-memory buffer)
    ↓
  SkImage (snapshot of rendered frame)
    ↓
  CCTexture2D (cocos2d-x texture from Skia render)
    ↓
  CCSprite (displays the texture)
    ↓
  WidgetsContainer (extends CCSprite)
    ↓
  Scene
```

### Key Components:

1. **PipelineOwner** - Manages render objects and paint phase
   - Tracks dirty RenderObjects
   - Manages SkCanvas lifecycle
   - Calls `flushPaint()` with SkCanvas* instead of CCNode*

2. **SkiaRenderContext** (NEW) - Wraps Skia rendering
   ```cpp
   class SkiaRenderContext {
     sk_sp<SkSurface> surface;      // In-memory render target
     SkCanvas* canvas;              // Drawing API
     SkImage* snapshot;             // Latest rendered frame
     
     void beginFrame(int width, int height);
     void endFrame();               // Produces SkImage
     SkImage* getSnapshot();
   };
   ```

3. **RenderObject.paint(SkCanvas*)** - Updated signature
   - Concrete subclasses paint using Skia API
   - No direct CCNode manipulation from RenderObjects

4. **WidgetsContainer extends CCSprite**
   ```cpp
   class WidgetsContainer : public CCSprite, public WidgetsBinding, public SchedulerBinding {
     SkiaRenderContext m_skiaContext;
     
     void init() {
       // Initialize sprite with window size (like CCLayer)
       setContentSize(CCDirector::sharedDirector()->getWinSize());
       setAnchorPoint(ccp(0, 0));
       initWidgetsBinding();
       scheduleUpdate();
     }
     
     void update(float deltaTime) {
       handleBeginFrame(++m_frameCount);
       m_schedulerPhase = SchedulerPhase::midFrameMicrotasks;
       handleDrawFrame();
       paintRenderTree();  // Render widget tree to Skia
     }
     
     void paintRenderTree();        // Layouts and paints render tree to Skia canvas
     void updateSpriteTexture();    // Updates CCSprite texture from SkImage snapshot
   };
   ```

## Frame Flow (NEW with Skia)

```
WidgetsContainer.update() [each frame]
  ↓
handleBeginFrame()
  ↓
handleDrawFrame() [persistent callbacks - element tree rebuild hooks]
  ↓
paintRenderTree()
  ├→ Get child render object from root element
  ├→ RenderObject.layout(constraints)
  ├→ SkiaRenderContext.beginFrame()
  ├→ RenderObject.paint(SkCanvas*)
  │  └→ Skia drawing commands (no CCNode creation)
  ├→ SkiaRenderContext.endFrame()
  ↓
updateSpriteTexture()
  ├→ SkImage::readPixels() to extract pixel data
  ├→ Create CCTexture2D from pixel data (RGBA8888)
  ├→ Set sprite texture and rect
  ↓
cocos2d renders CCSprite as normal
```

## Implementation Changes

### 1. CMake Setup Changes
```cmake
# Add Skia prebuilt binaries
# Download from: https://github.com/HenrySck075/geode-skia/releases
# Extract to: skia/[GEODE_TARGET_PLATFORM]/ (and move skia include folder to a subfolder also named skia)
# Link Skia libraries and include directories
```

### 2. RenderObject Signature Change
```cpp
// OLD
virtual void paint(cocos2d::CCNode* parent) = 0;

// NEW
virtual void paint(SkCanvas* canvas) = 0;
```

### 3. Example Render Objects (Skia version)

**ColoredBoxRender::paint(SkCanvas* canvas):**
```cpp
SkPaint paint;
paint.setColor(skColor);  // Convert ccColor3B to SkColor
canvas->drawRect(SkRect::MakeWH(size.width, size.height), paint);
```

**PaddingRender::paint(SkCanvas* canvas):**
```cpp
SkCanvas::AutoCanvasRestore acr(canvas, true);
canvas->translate(padding, padding);
if (child) child->paint(canvas);
```

### 4. New Classes

**include/gdlazer/caffeine/foundation/SkiaRenderContext.hpp**
```cpp
class SkiaRenderContext {
public:
  void beginFrame(int width, int height);
  void endFrame();
  SkCanvas* getCanvas();
  sk_sp<SkImage> getSnapshot();
};
```

**include/gdlazer/caffeine/foundation/PipelineOwner.hpp**
```cpp
class PipelineOwner {
  void flushLayout();
  void flushPaint(SkCanvas* canvas);
  void markNeedsPaint(...);
};
```

### 5. WidgetsContainer Changes
```cpp
class WidgetsContainer : public CCSprite, public WidgetsBinding, public SchedulerBinding {
  SkiaRenderContext m_skiaContext;
  
  void update(float deltaTime) override;
  void paintRenderTree();
  void updateSpriteTexture();
};
```

## Files to Modify/Create

### NEW Files:
- [ ] `include/gdlazer/caffeine/foundation/SkiaRenderContext.hpp`
- [ ] `src/caffeine/foundation/SkiaRenderContext.cpp`
- [ ] `include/gdlazer/caffeine/foundation/PipelineOwner.hpp`
- [ ] `src/caffeine/foundation/PipelineOwner.cpp`

### MODIFY Files:
- [ ] `CMakeLists.txt` - Add Skia dependency and linking
- [ ] `include/gdlazer/caffeine/foundation/RenderObject.hpp` - Change paint() signature
- [ ] `include/gdlazer/caffeine/foundation/RenderObjectElements.hpp` - Update element code
- [ ] `src/caffeine/foundation/RenderObjectElements.cpp`
- [ ] `include/gdlazer/caffeine/widgets/WidgetsBinding.hpp` - CCSprite base, add PipelineOwner
- [ ] `src/caffeine/widgets/WidgetsBinding.cpp` - Implement paint pipeline
- [ ] `src/example_mod.cpp` - Update render objects to use SkCanvas

### INFRASTRUCTURE:
- ~~[ ] `CMakeModules/FindSkia.cmake` - Custom CMake module for Skia discovery/download~~ geode-skia is specifically made for this project dont do this

## Key Implementation Details

### Canvas Lifecycle Per Frame:
```cpp
void WidgetsContainer::update(float deltaTime) {
  CCSprite::update(deltaTime);
  
  m_skiaContext.beginFrame(width, height);  // Create surface/canvas
  {
    handleBeginFrame(++m_frameCount);
    m_schedulerPhase = SchedulerPhase::midFrameMicrotasks;
    handleDrawFrame();                        // Rebuild elements
    
    m_pipelineOwner->flushLayout();          // Layout phase
    m_pipelineOwner->flushPaint(             // Paint phase
      m_skiaContext.getCanvas()
    );
  }
  m_skiaContext.endFrame();                  // Snapshot
  updateSpriteTexture();                     // Update CCSprite texture
}
```

### Skia to CCTexture2D Conversion:
```cpp
void WidgetsContainer::updateSpriteTexture() {
  sk_sp<SkImage> image = m_skiaContext.getSnapshot();
  
  // Convert SkImage to raw pixels
  SkPixmap pixmap;
  image->peekPixels(&pixmap);
  
  // Create CCTexture2D from pixel data
  CCTexture2D* texture = new CCTexture2D();
  texture->initWithData(
    pixmap.addr(),
    pixmap.rowBytes() * pixmap.height(),
    kCCTexture2DPixelFormat_RGBA8888,
    pixmap.width(),
    pixmap.height()
  );
  
  // Update sprite
  this->setDisplayFrame(
    CCSpriteFrame::frameWithTexture(texture, CCRectMake(...))
  );
}
```

## Benefits of This Approach

| Aspect | Benefit |
|--------|---------|
| **Rendering Quality** | Skia provides high-quality 2D rendering |
| **Clipping Support** | Native Skia clipping for future ClipRect widget |
| **Performance** | Off-screen rendering, minimal CCNode overhead |
| **Flexibility** | Can use advanced Skia features (gradients, paths, etc.) |
| **Proven** | Flutter uses this exact approach |
| **Decoupling** | RenderObjects don't know about cocos2d |

## Migration Path

1. **Phase 1** - Setup Skia in CMake, create SkiaRenderContext
2. **Phase 2** - Update RenderObject paint() signature to use SkCanvas*
3. **Phase 3** - Change WidgetsContainer to extend CCSprite
4. **Phase 4** - Implement PipelineOwner with flushPaint(SkCanvas*)
5. **Phase 5** - Update example render objects to paint with Skia
6. **Phase 6** - Test and optimize

## Notes

- Skia coordinate system: origin (0,0) at top-left, y-down (matches cocos2d)
- SkCanvas is managed by SkSurface, don't delete manually
- SkImage snapshot is cheap, can be called every frame
- CCTexture2D creation is where the cost is (consider caching if needed)
- Skia colors: `SkColorSetARGB(alpha, red, green, blue)` format

---

## Build Status - Session 7 (Skia Integration Complete)

**Status**: ✅ COMPLETE - Skia rendering pipeline fully integrated

### Completed:
✅ SkiaRenderContext.hpp and .cpp - Manages off-screen Skia surface and snapshots
✅ WidgetsContainer changed from CCLayer to CCSprite
✅ WidgetsContainer.init() - Initializes sprite with window size, anchor point at origin
✅ WidgetsContainer.paintRenderTree() - Layouts and renders to Skia canvas
✅ WidgetsContainer.updateSpriteTexture() - Converts SkImage to CCTexture2D
✅ RootElement.getChildRenderObject() - Exposes child render object for painting
✅ Frame pipeline - update() → handleBeginFrame() → handleDrawFrame() → paintRenderTree()

### Architecture:
- RenderObjects paint directly to SkCanvas (no CCNode tree creation)
- Skia renders to in-memory raster surface
- Each frame: snapshot → readPixels → CCTexture2D → CCSprite
- Window-sized sprite (0,0) origin, full coverage

### Key Implementation Details:
- `SkiaRenderContext::beginFrame()` creates/clears raster surface
- `SkiaRenderContext::endFrame()` snapshots to SkImage
- Pixel extraction via `SkImage::readPixels()` (RGBA8888)
- CCTexture2D created with pixel data each frame
- paintRenderTree() called every frame in update()

### Next Steps:
- Implement element tree rebuild via persistent frame callbacks
- Optimize texture caching if needed
- Add dirty tracking to skip unnecessary renders
- Implement more RenderObject subclasses as needed

