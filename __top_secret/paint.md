# Flutter Paint Update Flow: From `markNeedsPaint()` to Screen Render

This document traces the **exact source-code-level flow** of how Flutter goes from a `RenderObject.markNeedsPaint()` call to actually getting the paint updated on the next frame.

## Overview Flow Diagram

```
markNeedsPaint()
    ↓
(Sets _needsPaint flag)
    ↓
markNeedsPaint() on parent OR add to owner._nodesNeedingPaint
    ↓
owner.requestVisualUpdate()
    ↓
RendererBinding.ensureVisualUpdate()
    ↓
SchedulerBinding.scheduleFrame()
    ↓
platformDispatcher.scheduleFrame() [native engine]
    ↓
Vsync signal from OS (display refresh)
    ↓
Engine calls SchedulerBinding._handleBeginFrame()
    ↓
SchedulerBinding.handleBeginFrame() [animation phase]
    ↓
Engine calls SchedulerBinding.handleDrawFrame()
    ↓
RendererBinding._handlePersistentFrameCallback() [persistent frame callback]
    ↓
RendererBinding.drawFrame()
    ↓
PipelineOwner.flushPaint()
    ↓
Iterate _nodesNeedingPaint and call PaintingContext.repaintCompositedChild()
    ↓
RenderObject._paintWithContext()
    ↓
RenderObject.paint() [user-defined paint logic]
    ↓
compositeFrame() → GPU receives scene
```

---

## Detailed Step-by-Step Breakdown

### Step 1: `RenderObject.markNeedsPaint()` - Setting the Flag

**File:** `rendering/object.dart:3187`

```dart
void markNeedsPaint() {
  assert(!_debugDisposed);
  assert(owner == null || !owner!.debugDoingPaint);
  if (_needsPaint) {
    return;  // Early return: already marked
  }
  _needsPaint = true;  // ← FLAG SET HERE
  
  // If this was not previously a repaint boundary it will not have
  // a layer we can paint from.
  if (isRepaintBoundary && _wasRepaintBoundary) {
    // ... REPAINT BOUNDARY PATH ...
  } else if (parent != null) {
    // ... NON-REPAINT BOUNDARY PATH ...
  }
}
```

**Key observation:** `markNeedsPaint()` only sets a boolean flag (`_needsPaint = true`). It does **NOT** immediately trigger painting.

---

### Step 2: Routing the Paint Request

There are two critical paths depending on whether the RenderObject is a **repaint boundary**:

#### Path A: Repaint Boundary RenderObjects

**File:** `rendering/object.dart:3187-3214`

When `isRepaintBoundary && _wasRepaintBoundary` is true:

```dart
if (isRepaintBoundary && _wasRepaintBoundary) {
  assert(
    debugPrintMarkNeedsPaintStacks,
    debugPrintStack(label: 'markNeedsPaint() called for $this'),
  );
  // If we always have our own layer, then we can just repaint
  // ourselves without involving any other nodes.
  assert(_layerHandle.layer is OffsetLayer);
  if (owner != null) {
    owner!._nodesNeedingPaint.add(this);  // ← ADD TO OWNER'S PAINT LIST
    owner!.requestVisualUpdate();         // ← SCHEDULE VISUAL UPDATE
  }
}
```

**What happens:**
1. The RenderObject is added to `owner._nodesNeedingPaint` list (a queue of dirty nodes)
2. `requestVisualUpdate()` is called on the owner (PipelineOwner)

#### Path B: Non-Repaint Boundary RenderObjects

**File:** `rendering/object.dart:3211-3226`

When the object is not a repaint boundary:

```dart
else if (parent != null) {
  parent!.markNeedsPaint();  // ← PROPAGATE UP THE TREE
} else {
  // Root of render tree
  owner?.requestVisualUpdate();
}
```

**What happens:**
- The call propagates **upward through the parent chain** until reaching a repaint boundary
- Eventually calls `requestVisualUpdate()` on a repaint boundary or root

---

### Step 3: `owner.requestVisualUpdate()` - Signal to Scheduler

**File:** `rendering/binding.dart:828-830`

The PipelineOwner's `requestVisualUpdate()` method:

```dart
void requestVisualUpdate() {
  _binding.ensureVisualUpdate();  // ← Delegates to RendererBinding
}
```

This calls through to the RendererBinding.

---

### Step 4: `ensureVisualUpdate()` - Check Scheduler Phase

**File:** `scheduler/binding.dart:906-917`

```dart
void ensureVisualUpdate() {
  switch (schedulerPhase) {
    case SchedulerPhase.idle:
    case SchedulerPhase.postFrameCallbacks:
      scheduleFrame();  // ← SCHEDULE NEW FRAME
      return;
    case SchedulerPhase.transientCallbacks:
    case SchedulerPhase.midFrameMicrotasks:
    case SchedulerPhase.persistentCallbacks:
      return;  // Frame already being processed or scheduled
  }
}
```

**Key insight:** The method checks if a frame is already being processed. If we're already in a frame cycle (transient, midFrame, or persistent callbacks), it's a no-op. Otherwise, it schedules a new frame.

---

### Step 5: `scheduleFrame()` - Request Native Frame

**File:** `scheduler/binding.dart:946-959`

```dart
void scheduleFrame() {
  if (_hasScheduledFrame || !framesEnabled) {
    return;
  }
  assert(() {
    if (debugPrintScheduleFrameStacks) {
      debugPrintStack(label: 'scheduleFrame() called. Current phase is $schedulerPhase.');
    }
    return true;
  }());
  ensureFrameCallbacksRegistered();
  platformDispatcher.scheduleFrame();  // ← NATIVE ENGINE CALL
  _hasScheduledFrame = true;
}
```

**What happens:**
1. Checks if a frame is already scheduled via `_hasScheduledFrame`
2. Registers the frame callbacks with the platform dispatcher
3. Calls the native engine's `platformDispatcher.scheduleFrame()` to request a Vsync signal

---

### Step 6: Vsync & Frame Trigger - OS Signals Display Refresh

The native engine waits for the next Vsync signal from the operating system (display refresh, typically 16.67ms on 60Hz displays or 8.33ms on 120Hz).

When Vsync occurs, the engine calls back into Dart:

**File:** `scheduler/binding.dart:889`

```dart
platformDispatcher.onBeginFrame ??= _handleBeginFrame;
```

---

### Step 7: `_handleBeginFrame()` → `handleBeginFrame()` - Animation Phase

**File:** `scheduler/binding.dart:1168-1177`

```dart
void _handleBeginFrame(Duration rawTimeStamp) {
  if (_warmUpFrame) {
    _scheduleWarmUpFrameCheck();
  }
  handleBeginFrame(rawTimeStamp);
}
```

Then **`handleBeginFrame()`** (lines 1226-1274) handles the "animation phase":

```dart
void handleBeginFrame(Duration? rawTimeStamp) {
  _frameTimelineTask?.start('Frame');
  _firstRawTimeStampInEpoch ??= rawTimeStamp;
  _currentFrameTimeStamp = _adjustForEpoch(rawTimeStamp ?? _lastRawTimeStamp);
  
  assert(schedulerPhase == SchedulerPhase.idle);
  _hasScheduledFrame = false;  // ← Frame is now being processed
  try {
    // TRANSIENT FRAME CALLBACKS
    _schedulerPhase = SchedulerPhase.transientCallbacks;
    final Map<int, _FrameCallbackEntry> callbacks = _transientCallbacks;
    _transientCallbacks = <int, _FrameCallbackEntry>{};
    callbacks.forEach((int id, _FrameCallbackEntry callbackEntry) {
      if (!_removedIds.contains(id)) {
        _invokeFrameCallback(/* ... */);  // ← Runs animations, Tickers, etc.
      }
    });
    _removedIds.clear();
  } finally {
    _schedulerPhase = SchedulerPhase.midFrameMicrotasks;
  }
}
```

**What happens:**
1. Sets phase to `transientCallbacks` 
2. Runs all registered transient frame callbacks (Tickers, AnimationControllers)
3. Updates animation values
4. Sets phase to `midFrameMicrotasks` to allow microtasks to run

---

### Step 8: Engine Calls `handleDrawFrame()` - Persistent Callbacks Phase

After microtasks drain, the engine calls:

**File:** `scheduler/binding.dart:1338-1360`

```dart
void handleDrawFrame() {
  assert(_schedulerPhase == SchedulerPhase.midFrameMicrotasks);
  _frameTimelineTask?.finish();  // end the "Animate" phase
  try {
    // PERSISTENT FRAME CALLBACKS
    _schedulerPhase = SchedulerPhase.persistentCallbacks;
    for (final callback in List<FrameCallback>.of(_persistentCallbacks)) {
      _invokeFrameCallback(callback, _currentFrameTimeStamp!);
    }
    // ... POST-FRAME CALLBACKS phase ...
  } finally {
    _schedulerPhase = SchedulerPhase.idle;
    _currentFrameTimeStamp = null;
  }
}
```

The persistent frame callbacks are **where the magic happens for rendering**.

---

### Step 9: `_handlePersistentFrameCallback()` - Rendering Pipeline Entry

**File:** `rendering/binding.dart:61 and 494-497`

During `initInstances()`, RendererBinding registers:

```dart
addPersistentFrameCallback(_handlePersistentFrameCallback);
```

When the persistent phase runs, this callback is invoked:

```dart
void _handlePersistentFrameCallback(Duration timeStamp) {
  drawFrame();  // ← THE MAIN RENDERING FUNCTION
  _scheduleMouseTrackerUpdate();
}
```

---

### Step 10: `drawFrame()` - Execute Rendering Pipeline

**File:** `rendering/binding.dart:628-639`

This is where all rendering phases happen:

```dart
void drawFrame() {
  rootPipelineOwner.flushLayout();           // Phase 3: Layout
  rootPipelineOwner.flushCompositingBits();  // Phase 4: Compositing bits
  rootPipelineOwner.flushPaint();            // Phase 5: PAINT ← HERE!
  if (sendFramesToEngine) {
    for (final RenderView renderView in renderViews) {
      renderView.compositeFrame();  // Phase 6: Compositing → GPU
    }
    rootPipelineOwner.flushSemantics();  // Phase 7: Semantics
    _firstFrameSent = true;
  }
}
```

Our paint happens in `flushPaint()`.

---

### Step 11: `PipelineOwner.flushPaint()` - Process Dirty Paint List

**File:** `rendering/object.dart:1293-1350`

This is the core paint flushing logic:

```dart
void flushPaint() {
  if (!kReleaseMode) {
    // ... Timeline tracking ...
    FlutterTimeline.startSync('PAINT...');
  }
  try {
    assert(() {
      _debugDoingPaint = true;
      return true;
    }());
    final List<RenderObject> dirtyNodes = _nodesNeedingPaint;
    _nodesNeedingPaint = <RenderObject>[];  // ← SWAP LIST: old list becomes working list

    // Sort the dirty nodes in reverse order (deepest first).
    for (final node in dirtyNodes..sort((RenderObject a, RenderObject b) => b.depth - a.depth)) {
      assert(node._layerHandle.layer != null);
      if ((node._needsPaint || node._needsCompositedLayerUpdate) && node.owner == this) {
        if (node._layerHandle.layer!.attached) {
          assert(node.isRepaintBoundary);
          if (node._needsPaint) {
            PaintingContext.repaintCompositedChild(node);  // ← REPAINT THIS NODE
          } else {
            PaintingContext.updateLayerProperties(node);
          }
        } else {
          node._skippedPaintingOnLayer();
        }
      }
    }
    for (final PipelineOwner child in _children) {
      child.flushPaint();  // ← Recursively flush child pipeline owners
    }
    assert(
      _nodesNeedingPaint.isEmpty,
      'Child PipelineOwners must not dirty nodes in their parent.',
    );
  } finally {
    assert(() {
      _debugDoingPaint = false;
      return true;
    }());
    if (!kReleaseMode) {
      FlutterTimeline.finishSync();
    }
  }
}
```

**Key insights:**
1. **Swaps the list:** `dirtyNodes = _nodesNeedingPaint; _nodesNeedingPaint = []` – new dirty nodes registered during painting don't interfere
2. **Sorts by depth:** Deepest nodes first (reverse sort) ensures children paint before parents
3. **Checks flags:** Only paints nodes where `_needsPaint || _needsCompositedLayerUpdate`
4. **Checks attachment:** Only paints if the layer is still attached to the layer tree
5. **Calls `repaintCompositedChild()`** for each dirty repaint boundary

---

### Step 12: `PaintingContext.repaintCompositedChild()` - Invoke Paint

**File:** `rendering/object.dart:123-186`

```dart
static void repaintCompositedChild(RenderObject child, {bool debugAlsoPaintedParent = false}) {
  assert(child._needsPaint);
  _repaintCompositedChild(child, debugAlsoPaintedParent: debugAlsoPaintedParent);
}

static void _repaintCompositedChild(
  RenderObject child, {
  bool debugAlsoPaintedParent = false,
  PaintingContext? childContext,
}) {
  assert(child.isRepaintBoundary);
  
  // Get or create the OffsetLayer for this repaint boundary
  var childLayer = child._layerHandle.layer as OffsetLayer?;
  if (childLayer == null) {
    final OffsetLayer layer = child.updateCompositedLayer(oldLayer: null);
    child._layerHandle.layer = childLayer = layer;
  } else {
    Offset? debugOldOffset;
    childLayer.removeAllChildren();  // ← CLEAR OLD PAINT COMMANDS
    final OffsetLayer updatedLayer = child.updateCompositedLayer(oldLayer: childLayer);
    assert(identical(updatedLayer, childLayer));
  }
  child._needsCompositedLayerUpdate = false;

  assert(identical(childLayer, child._layerHandle.layer is OffsetLayer));
  
  // Create painting context for this layer
  childContext ??= PaintingContext(childLayer, child.paintBounds);
  
  // THIS IS IT: Call the RenderObject's paint method
  child._paintWithContext(childContext, Offset.zero);
  
  // Ensure the layer wasn't replaced
  assert(identical(childLayer, child._layerHandle.layer));
  childContext.stopRecordingIfNeeded();
}
```

**Critical points:**
1. Gets or creates an `OffsetLayer` (the composited layer for this repaint boundary)
2. **Clears old paint commands** via `childLayer.removeAllChildren()`
3. Creates a `PaintingContext` (a Canvas wrapper)
4. **Calls `child._paintWithContext()`** which invokes the actual `paint()` method

---

### Step 13: `RenderObject._paintWithContext()` - Execute User Paint Code

**File:** `rendering/object.dart` (internal method that calls `paint()`)

This internal method:
1. Creates a new `ui.PictureRecorder` 
2. Creates a `Canvas` connected to the recorder
3. Calls the user-implemented `paint(context, offset)` method
4. Records all the drawing commands into the picture

The user's custom `paint()` method receives:
- **`PaintingContext context`** – wraps the canvas for painting
- **`Offset offset`** – offset to apply to all coordinates

Example paint implementation:

```dart
@override
void paint(PaintingContext context, Offset offset) {
  context.canvas.drawRect(
    Rect.fromLTWH(offset.dx, offset.dy, size.width, size.height),
    Paint()..color = Colors.blue,
  );
}
```

These canvas draw commands are recorded into the layer's picture.

---

### Step 14: `compositeFrame()` - Send Scene to GPU

Back in `drawFrame()` after `flushPaint()`:

**File:** `rendering/binding.dart:633-635`

```dart
if (sendFramesToEngine) {
  for (final RenderView renderView in renderViews) {
    renderView.compositeFrame();  // ← Convert layer tree to scene and send to GPU
  }
}
```

`compositeFrame()` converts the entire layer tree (containing all the recorded pictures) into a `Scene` object and sends it to the GPU for rendering.

---

## Summary: The Complete Flow

| Step | Method | File | Action |
|------|--------|------|--------|
| 1 | `markNeedsPaint()` | rendering/object.dart:3187 | Sets `_needsPaint = true` |
| 2 | `markNeedsPaint()` (continued) | rendering/object.dart:3207 | Adds to `owner._nodesNeedingPaint` list |
| 3 | `requestVisualUpdate()` | rendering/binding.dart:829 | Delegates to RendererBinding |
| 4 | `ensureVisualUpdate()` | scheduler/binding.dart:906 | Checks scheduler phase |
| 5 | `scheduleFrame()` | scheduler/binding.dart:946 | Calls native `platformDispatcher.scheduleFrame()` |
| 6 | — | (Native layer) | OS sends Vsync signal on next display refresh |
| 7 | `_handleBeginFrame()` → `handleBeginFrame()` | scheduler/binding.dart:1168-1274 | Animation phase: ticks animations |
| 8 | `handleDrawFrame()` | scheduler/binding.dart:1338 | Persistent callbacks phase begins |
| 9 | `_handlePersistentFrameCallback()` | rendering/binding.dart:494 | Calls `drawFrame()` |
| 10 | `drawFrame()` | rendering/binding.dart:628 | Calls `flushLayout()`, `flushCompositingBits()`, **`flushPaint()`** |
| 11 | `flushPaint()` | rendering/object.dart:1293 | Iterates `_nodesNeedingPaint`, calls `repaintCompositedChild()` |
| 12 | `_repaintCompositedChild()` | rendering/object.dart:128 | Creates `PaintingContext`, calls `_paintWithContext()` |
| 13 | `_paintWithContext()` | rendering/object.dart | Invokes user's `paint(context, offset)` method |
| 14 | `compositeFrame()` | rendering/binding.dart:634 | Converts layer tree to Scene, sends to GPU |

---

## Key Synchronization Points

1. **Deduplication:** Early return in `markNeedsPaint()` if `_needsPaint` already true
2. **Scheduler awareness:** `ensureVisualUpdate()` is a no-op if already in a frame cycle
3. **List swapping:** `flushPaint()` swaps dirty list to prevent interference from repaints scheduled during painting
4. **Depth sorting:** Nodes painted deepest-first (children before parents) via reverse depth sort
5. **Layer attachment check:** Only paints nodes whose layers are still attached

---

## Timeline in Real Time (60 Hz Display)

```
Frame N:
  t=0ms:     markNeedsPaint() called
  t=0ms:     owner.requestVisualUpdate() called
  t=0-16ms:  Waiting for Vsync
  t=16ms:    Vsync signal from OS
  t=16ms:    Engine calls handleBeginFrame() [animations tick]
  t=16ms:    Microtasks drain
  t=16ms:    Engine calls handleDrawFrame()
  t=16ms:    _handlePersistentFrameCallback() runs
  t=16ms:    drawFrame() executes:
             - flushLayout()
             - flushCompositingBits()
             - flushPaint() ← Paint happens here!
             - compositeFrame() ← GPU gets scene
  t=16ms:    Scene displayed on screen next Vsync (t=32ms)
```

Actually appears on screen at the **next Vsync** after `compositeFrame()` completes.

---

## What Does NOT Trigger Immediate Paint

- Setting `_needsPaint = true` alone
- Calling `markNeedsPaint()` during a current frame (just queues for next frame)
- The painter function is NOT called synchronously in `markNeedsPaint()`

---

## Optimization: Early Out Mechanisms

1. **`if (_needsPaint) return`** – Prevents duplicate markings
2. **`if (_hasScheduledFrame) return`** – Prevents scheduling multiple times
3. **Scheduler phase checks** – Avoids redundant scheduling during active frame
4. **Repaint boundary optimization** – Only repaint boundary nodes are queued; non-boundaries propagate marking up
