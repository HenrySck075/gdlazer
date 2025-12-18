# C++ Port Paint Architecture Changes (Flutter paint.md Analysis)

## Key Insights from Flutter's Paint Flow

Flutter's painting sequence has critical characteristics we must replicate:

1. **Deferred Painting**: `markNeedsPaint()` only sets a flag—**doesn't paint immediately**
2. **Centralized Scheduling**: All dirty paints queued into `owner._nodesNeedingPaint` list
3. **Depth-sorted Execution**: Nodes painted deepest-first (children before parents)
4. **Layer-based Rendering**: Each repaint boundary gets its own layer/canvas
5. **Single Frame Cycle**: All paints happen in one phase (`flushPaint()`), then composited once
6. **Canvas Swap Pattern**: Dirty list swapped during flush to avoid interference during painting

---

## Current C++ Port Issues

Looking at `RenderObject.hpp` and `SkiaRenderContext.hpp`:

- ✅ `markNeedsPaint()` flag exists but **unused**
- ❌ No paint queue/list system (nodes don't register themselves)
- ❌ `paint()` signature takes `SkCanvas*` directly—no `PaintingContext` wrapper
- ❌ No frame-synchronized painting (when does `paint()` actually get called?)
- ❌ `SkiaRenderContext::getCanvas()` returns single canvas—**shared across all nodes**
- ❌ No owner/pipeline orchestration (who calls `flushPaint()`?)
- ❌ No depth-sorting or dirty-node-list traversal

---

## Required Changes

### 1. **Create PaintingContext Abstraction** (new header)
**File**: `include/gdlazer/caffeine/rendering/painting_context.hpp`

```cpp
class PaintingContext {
private:
  SkCanvas* m_canvas;
  Rect m_paintBounds;
  
public:
  PaintingContext(SkCanvas* canvas, const Rect& paintBounds)
    : m_canvas(canvas), m_paintBounds(paintBounds) {}
  
  SkCanvas* getCanvas() const { return m_canvas; }
  const Rect& getPaintBounds() const { return m_paintBounds; }
};
```

**Why**: Mirrors Flutter's `PaintingContext` which wraps the canvas. Provides abstraction for future layer/compositing logic.

---

### 2. **Update RenderObject Paint Signature**
**File**: `include/gdlazer/caffeine/rendering/object/RenderObject.hpp`

**CHANGE**:
```cpp
// OLD:
virtual void paint(SkCanvas* canvas) = 0;

// NEW:
virtual void paint(PaintingContext* context, const Offset& offset) = 0;
```

**Why**: Matches Flutter's signature. `offset` allows repositioning (child offset from parent). `PaintingContext` provides abstraction.

---

### 3. **Create PipelineOwner with Paint Flushing**
**File**: `include/gdlazer/caffeine/rendering/pipeline_owner.hpp`

```cpp
class PipelineOwner {
private:
  std::vector<RenderObject*> m_nodesNeedingPaint;
  SkiaRenderContext* m_renderContext;
  
public:
  void addDirtyPaint(RenderObject* node);
  void flushPaint();  // Main paint orchestration
  void requestVisualUpdate();  // Called by markNeedsPaint()
};
```

**Key Methods**:
- `addDirtyPaint()`: Add node to `m_nodesNeedingPaint` queue
- `flushPaint()`: 
  1. Sort nodes by depth (deepest first)
  2. Swap dirty list
  3. Iterate and call `paint()` on each
  4. Handle layer/canvas management per node

**Why**: Centralizes paint lifecycle, orchestrates frame-wide painting.

---

### 4. **Update RenderObject to Use PipelineOwner**
**File**: `include/gdlazer/caffeine/rendering/object/RenderObject.hpp`

**ADD**:
```cpp
class RenderObject : public log::StringConvertible, public cocos2d::CCObject {
protected:
  // ... existing ...
  PipelineOwner* m_owner = nullptr;  // NEW
  
public:
  void markNeedsPaint() {
    if (!m_needsPaint) {
      m_needsPaint = true;
      if (m_owner) {
        m_owner->addDirtyPaint(this);  // QUEUE instead of just flag
      } else if (m_parent) {
        m_parent->markNeedsPaint();  // Propagate up
      }
    }
  }
  
  void setOwner(PipelineOwner* owner) { m_owner = owner; }
  PipelineOwner* getOwner() const { return m_owner; }
  
  // Paint execution (called by PipelineOwner::flushPaint)
  void _paintWithContext(PaintingContext* context, const Offset& offset);
};
```

**Why**: 
- Defers to owner instead of calling paint immediately
- Propagates up tree if no owner (non-repaint-boundary pattern)
- Allows owner to orchestrate painting

---

### 5. **Update SkiaRenderContext for Per-Frame Management**
**File**: `include/gdlazer/caffeine/rendering/SkiaRenderContext.hpp`

**CHANGE**:
```cpp
// OLD:
void beginFrame(int width, int height);
SkCanvas* getCanvas();

// NEW:
void beginFrame(int width, int height);
PaintingContext createPaintingContext(const Rect& bounds);
SkCanvas* getCanvasForFrame() const;
void endFrame();
```

**Why**: Each node should get its own `PaintingContext` with appropriate bounds, not share a canvas directly.

---

### 6. **Add RenderView Orchestration**
**File**: `include/gdlazer/caffeine/rendering/object/render_view.hpp` (new)

```cpp
class RenderView : public RenderObject {
private:
  PipelineOwner m_pipelineOwner;
  SkiaRenderContext m_renderContext;
  
public:
  void drawFrame() {
    // Parallel to Flutter's drawFrame():
    flushLayout();           // Phase 1
    flushCompositingBits();  // Phase 2
    flushPaint();            // Phase 3 ← Paint happens here
    compositeFrame();        // Phase 4 → GPU

    // user note: phase 1/2 might need to be stubbed, for phase 2 and compositeFrame uhh idk we have access to the Skia library so we need to do it differently from Flutter on these one
  }
  
private:
  void flushPaint() {
    m_renderContext.beginFrame(width, height);
    m_pipelineOwner.flushPaint();
    m_renderContext.endFrame();
  }
};
```

**Why**: Mirrors `RenderView.compositeFrame()` in Flutter. Single entry point for frame rendering.

---

## Implementation Strategy

### Phase 1: Minimal Changes (gets deferred painting working)
1. Create `PaintingContext` wrapper
2. Update `paint()` signature in `RenderObject`
3. Create basic `PipelineOwner` with `addDirtyPaint()` + `flushPaint()`
4. Hook `markNeedsPaint()` to queue via owner

### Phase 2: Frame Synchronization
5. Create `RenderView` with `drawFrame()` orchestration
6. Integrate `SkiaRenderContext` lifecycle with frame phases
7. Implement depth-sorting in `flushPaint()`

### Phase 3: Optimization
8. Implement layer caching per repaint boundary
9. Add dirty-list-swap pattern (prevent recursive marking during paint)

---

## Critical Differences from Current Code

| Aspect | Current | Should Be |
|--------|---------|-----------|
| **Canvas** | Single shared canvas | Per-node/per-bounds PaintingContext |
| **Paint Trigger** | Unknown (never actually called?) | Centralized `flushPaint()` during frame |
| **Dirty Tracking** | Flag only | Flag + queue in owner |
| **Paint Order** | Undefined | Depth-sorted, deepest-first |
| **Frame Lifecycle** | None | Explicit `drawFrame()` phases |
| **Ownership** | No owner/pipeline | RenderView + PipelineOwner |

---

## Notes on "Single Canvas" Issue

Current design passes `SkCanvas*` to every `paint()` method. This assumes:
- All nodes paint to same canvas
- No layer isolation
- All rendering happens immediately

**Flutter's approach**:
- Each repaint boundary gets its own layer (canvas)
- Paint commands recorded into pictures
- Composited together at end
- Allows selective invalidation & caching

**For C++ port minimum viable fix**:
1. Create `PaintingContext` to wrap canvas (abstraction)
2. Add node-specific offset to paint signature
3. Let `PipelineOwner.flushPaint()` manage which canvas each node paints to
4. Implement true layer isolation (probably??? the current test on example_mod.cpp uses StatefulWidget so)

---

## Example Flow After Changes

```
RenderObject::markNeedsPaint()
  ↓
Check owner exists → YES
  ↓
owner->addDirtyPaint(this)  [Queue node]
  ↓
// Later, in frame phase:
RenderView::drawFrame()
  ↓
pipelineOwner.flushPaint()
  ↓
For each dirty node (sorted by depth):
  PaintingContext ctx = renderContext.createPaintingContext(bounds);
  node->_paintWithContext(&ctx, offset);
  
  // Inside paint():
  context->getCanvas()->drawRect(...);
  ↓
renderContext.endFrame()
```

This defers all painting to a synchronized point, like Flutter does.


User note: WidgetsContainer currently directly inherits the bindings classes so we have to separate them to smth like idk WidgetsFlutterBinding i guess and let the container owns the bindings object
