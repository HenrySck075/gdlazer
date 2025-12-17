# Flutter Widget-Element-RenderObject Pipeline: Source Code Analysis

## Overview

Flutter's UI framework operates through three parallel hierarchies that work in synchronization:

1. **Widget Tree** (Immutable configuration objects)
2. **Element Tree** (Mutable, lifecycle-aware container objects)
3. **RenderObject Tree** (Actual layout, paint, and rendering operations)

This document traces how Flutter controls these trees at the source code level through the complete pipeline lifecycle.

---

## Part 1: Core Data Structures

### Widget (Immutable Configuration)

```dart
// From widgets/framework.dart
abstract class Widget {
  final Key? key;
  
  Element createElement();
  
  static bool canUpdate(Widget oldWidget, Widget newWidget) {
    return oldWidget.runtimeType == newWidget.runtimeType &&
           oldWidget.key == newWidget.key;
  }
}
```

**Key insight**: Widgets are pure configuration. They're immutable and never directly participate in rendering.

### Element (Mutable Lifecycle Container)

```dart
// From widgets/framework.dart ~line 3200+
abstract class Element implements BuildContext {
  Widget? _widget;
  Element? _parent;
  Object? _slot;
  
  BuildOwner? _owner;
  BuildScope? _parentBuildScope;
  
  _ElementLifecycle _lifecycleState = _ElementLifecycle.initial;
  
  bool _dirty = true;
  Set<InheritedElement>? _dependencies;
  
  // For RenderObjectElement subclasses
  RenderObject? _renderObject;
}

enum _ElementLifecycle {
  initial,    // Created but not mounted
  active,     // In the tree
  inactive,   // Removed but not disposed (may be reused)
  defunct,    // Permanently removed and disposed
}
```

**Key insight**: Elements are the actual "widgets" in the tree—they manage state, lifecycle, and ownership.

### RenderObject (Actual Rendering)

```dart
// From rendering/object.dart
abstract class RenderObject {
  ParentData? parentData;
  RenderObject? _parent;
  List<RenderObject> _children = [];
  
  void layout(Constraints constraints, {bool parentUsesSize = false});
  void paint(PaintingContext context, Offset offset);
  
  void markNeedsLayout();
  void markNeedsPaint();
  void markNeedsLayoutAndPaint();
}
```

**Key insight**: RenderObjects never know about Widgets. They only know about their parent RenderObjects and Constraints.

---

## Part 2: Mount Pipeline (Initial Tree Creation)

When a Widget enters the tree for the first time:

### Step 1: Widget Inflation

```dart
// From widgets/framework.dart ~line 4224
@protected
Element inflateWidget(Widget newWidget, Object? newSlot) {
  final Key? key = newWidget.key;
  
  // Check for GlobalKey reparenting opportunities
  final Element? inactiveChild = key is GlobalKey
      ? _retakeInactiveElement(key, newWidget)
      : null;
  
  // Create new Element OR reuse inactive one
  final Element newChild = inactiveChild ?? newWidget.createElement();
  
  try {
    if (inactiveChild != null) {
      // Reactivate path (GlobalKey reparenting)
      inactiveChild._activateWithParent(this, newSlot);
      return updateChild(inactiveChild, newWidget, newSlot)!;
    } else {
      // Fresh mount path
      newChild.mount(this, newSlot);  // ← ENTERS ACTIVE STATE HERE
      assert(newChild._lifecycleState == _ElementLifecycle.active);
      return newChild;
    }
  } catch (_) {
    _deactivateFailedChildSilently(newChild);
    rethrow;
  }
}
```

**What happens**: `newWidget.createElement()` creates the appropriate Element subclass (StatelessElement, StatefulElement, RenderObjectElement, etc.).

### Step 2: Element Mount

#### Base Element Mount

```dart
// From widgets/framework.dart ~line 4328
void mount(Element? parent, Object? newSlot) {
  assert(_lifecycleState == _ElementLifecycle.initial);
  assert(_parent == null);
  
  // 1. ESTABLISH PARENT LINK
  _parent = parent;
  _slot = newSlot;
  _lifecycleState = _ElementLifecycle.active;
  _depth = 1 + (parent?.depth ?? 0);
  
  // 2. CLAIM OWNERSHIP
  if (parent != null) {
    _owner = parent.owner;
    _parentBuildScope = parent.buildScope;
  }
  
  // 3. REGISTER GLOBAL KEYS
  final Key? key = widget.key;
  if (key is GlobalKey) {
    owner!._registerGlobalKey(key, this);
  }
  
  // 4. INHERIT DEPENDENCY TRACKING
  _updateInheritance();
  attachNotificationTree();
}
```

**Control point**: The Element now exists and is part of the tree hierarchy. Its `_owner` links it to the BuildOwner, which controls the build pipeline.

#### ComponentElement Mount (Stateless/Stateful Widgets)

```dart
// From widgets/framework.dart ~line 5789
void mount(Element? parent, Object? newSlot) {
  super.mount(parent, newSlot);  // Base Element setup
  assert(_child == null);
  assert(_lifecycleState == _ElementLifecycle.active);
  
  _firstBuild();  // ← CALLS rebuild() → performRebuild() → build()
  assert(_child != null);
}

void _firstBuild() {
  rebuild();  // This eventually calls performRebuild()
}
```

**Control point**: For component widgets, mounting triggers the first build.

#### RenderObjectElement Mount

```dart
// From widgets/framework.dart ~line 6784
void mount(Element? parent, Object? newSlot) {
  super.mount(parent, newSlot);
  
  // 1. CREATE RENDER OBJECT
  _renderObject = (widget as RenderObjectWidget).createRenderObject(this);
  assert(!_renderObject!.debugDisposed);
  
  // 2. ATTACH RENDER OBJECT TO PARENT
  attachRenderObject(newSlot);
  
  // 3. MARK AS CLEAN
  super.performRebuild();  // Clears _dirty flag
}
```

**Control point**: RenderObject creation happens here. The Element owns the RenderObject and controls its attachment to the parent's RenderObject.

#### SingleChildRenderObjectElement Mount

```dart
// From widgets/framework.dart ~line 7113
void mount(Element? parent, Object? newSlot) {
  super.mount(parent, newSlot);  // Creates RenderObject
  
  // UPDATE THE CHILD (which creates/mounts the child Element)
  _child = updateChild(_child, (widget as SingleChildRenderObjectWidget).child, null);
}
```

**Control point**: After the Element and its RenderObject are created, the child widget is processed through `updateChild()`.

#### MultiChildRenderObjectElement Mount

```dart
// From widgets/framework.dart ~line 7270
void mount(Element? parent, Object? newSlot) {
  super.mount(parent, newSlot);  // Creates RenderObject
  
  final multiChildRenderObjectWidget = widget as MultiChildRenderObjectWidget;
  final children = List<Element>.filled(
    multiChildRenderObjectWidget.children.length,
    _NullElement.instance,
  );
  
  Element? previousChild;
  for (var i = 0; i < children.length; i += 1) {
    // Inflate each child widget into an Element
    final Element newChild = inflateWidget(
      multiChildRenderObjectWidget.children[i],
      IndexedSlot<Element?>(i, previousChild),  // Slot links children in order
    );
    children[i] = newChild;
    previousChild = newChild;
  }
  
  _children = children;
}
```

**Control point**: Each child is inflated with an `IndexedSlot` that maintains its position in the parent's render object child list.

---

## Part 3: Build Pipeline (Widget → Element → RenderObject)

### Step 1: Mark Dirty

```dart
// From widgets/framework.dart ~line 5454
void markNeedsBuild() {
  assert(_lifecycleState != _ElementLifecycle.defunct);
  
  if (_lifecycleState != _ElementLifecycle.active) {
    return;
  }
  
  if (dirty) {
    return;  // Already marked, avoid redundant scheduling
  }
  
  _dirty = true;
  owner!.scheduleBuildFor(this);  // ← REGISTERS WITH BUILDOWNER
}
```

**What happens**: The Element is added to the BuildOwner's dirty list. This typically happens:
- When `State.setState()` is called
- When an InheritedWidget dependency changes
- When the parent widget updates

### Step 2: BuildOwner.buildScope (The Build Pass)

```dart
// From widgets/framework.dart ~line 2621
void buildScope(Element context, [VoidCallback? callback]) {
  final BuildScope buildScope = context.buildScope;
  
  if (callback == null && buildScope._dirtyElements.isEmpty) {
    return;
  }
  
  assert(!_debugBuilding);
  
  try {
    _debugBuilding = true;
    buildScope._building = true;
    
    if (callback != null) {
      callback();  // User code runs (e.g., setState callback)
    }
    
    buildScope._flushDirtyElements(debugBuildRoot: context);  // ← REBUILD ALL DIRTY ELEMENTS
    
  } finally {
    buildScope._building = false;
    _debugBuilding = false;
  }
}
```

**Key**: The entire build phase runs with `_debugBuilding = true`, preventing setState() from being called during build.

### Step 3: Element.rebuild()

```dart
// From widgets/framework.dart ~line 5503
void rebuild({bool force = false}) {
  assert(_lifecycleState != _ElementLifecycle.initial);
  
  // Early exit if not active or not dirty
  if (_lifecycleState != _ElementLifecycle.active || (!_dirty && !force)) {
    return;
  }
  
  assert(_lifecycleState == _ElementLifecycle.active);
  assert(owner!._debugStateLocked);  // setState() is forbidden now
  
  try {
    performRebuild();  // ← SUBCLASS-SPECIFIC REBUILD
  } finally {
    assert(!_dirty);  // Should be cleared by performRebuild
  }
}
```

**Control point**: This is where the rebuild decision is made. If `_dirty` is false and `force` is false, rebuild is skipped.

### Step 4: ComponentElement.performRebuild (Stateless/Stateful)

```dart
// From widgets/framework.dart ~line 5810
void performRebuild() {
  Widget built;
  
  try {
    // 1. CALL USER'S BUILD METHOD
    built = build();  // Calls StatelessWidget.build() or State.build()
    debugWidgetBuilderValue(widget, built);
  } catch (e, stack) {
    // Errors show ErrorWidget
    built = ErrorWidget.builder(...);
  } finally {
    // 2. MARK THIS ELEMENT AS CLEAN
    super.performRebuild();  // Sets _dirty = false
  }
  
  // 3. UPDATE THE BUILT CHILD WITH THE RETURNED WIDGET
  try {
    _child = updateChild(_child, built, slot);
    assert(_child != null);
  } catch (e, stack) {
    // Error recovery
    built = ErrorWidget.builder(...);
    _child?.deactivate();
    _child = updateChild(null, built, slot);
  }
}

// Abstract method implemented by subclasses
@protected
Widget build();
```

**What happens**: 
1. The user's build method is called (this is where UI is described)
2. The returned Widget is compared against the old child Element
3. The child Element is either created, updated, or removed

### Step 5: RenderObjectElement.performRebuild (Layout/Paint Updates)

```dart
// From widgets/framework.dart ~line 6825
void performRebuild() {
  _performRebuild();  // Calls widget.updateRenderObject()
}

void _performRebuild() {
  assert(_renderObject != null);
  
  // UPDATE THE RENDER OBJECT with new configuration
  (widget as RenderObjectWidget).updateRenderObject(this, renderObject);
  
  // MARK THIS ELEMENT AS CLEAN
  super.performRebuild();  // Sets _dirty = false
}
```

**What happens**: The RenderObject's properties are updated (color, padding, text, etc.) but the tree structure is not changed here.

---

## Part 4: Update Pipeline (Widget Change Detection)

When a parent updates its child configuration:

### Step 1: updateChild() - The Core Reconciliation

```dart
// From widgets/framework.dart ~line 3608
Element? updateChild(Element? child, Widget? newWidget, Object? newSlot) {
  // Case 1: Remove child if no new widget
  if (newWidget == null) {
    if (child != null) {
      deactivateChild(child);
    }
    return null;
  }
  
  final Element newChild;
  
  if (child != null) {
    // CHECK: Can we reuse the old element?
    if (hasSameSuperclass && child.widget == newWidget) {
      // NO CHANGE: Same widget instance, just update slot
      if (child.slot != newSlot) {
        updateSlotForChild(child, newSlot);
      }
      newChild = child;
      
    } else if (hasSameSuperclass && Widget.canUpdate(child.widget, newWidget)) {
      // SAME TYPE & KEY: Update the element's widget
      if (child.slot != newSlot) {
        updateSlotForChild(child, newSlot);
      }
      child.update(newWidget);  // ← CALLS element.update()
      newChild = child;
      
    } else {
      // TYPE or KEY CHANGED: Discard old element, create new one
      deactivateChild(child);
      newChild = inflateWidget(newWidget, newSlot);
    }
  } else {
    // No old element: Create a new one
    newChild = inflateWidget(newWidget, newSlot);
  }
  
  return newChild;
}
```

**Key logic**:
- `child.widget == newWidget`: Same object reference → No rebuild needed
- `Widget.canUpdate(old, new)`: Same type and key → Reuse element, call `update()`
- Otherwise → Destroy old element, create new one (expensive!)

### Step 2: Element.update()

```dart
// From widgets/framework.dart ~line 4375
void update(covariant Widget newWidget) {
  assert(
    _lifecycleState == _ElementLifecycle.active &&
    newWidget != widget &&
    Widget.canUpdate(widget, newWidget),
  );
  
  _widget = newWidget;  // Update the configuration
}
```

**For StatelessElement**:
```dart
@override
void update(StatelessWidget newWidget) {
  super.update(newWidget);
  assert(widget == newWidget);
  rebuild(force: true);  // Force rebuild with new widget
}
```

**For StatefulElement**:
```dart
@override
void update(StatefulWidget newWidget) {
  super.update(newWidget);
  final StatefulWidget oldWidget = state._widget!;
  state._widget = widget;
  state.didUpdateWidget(oldWidget);  // Notify the State
  rebuild(force: true);
}
```

**For RenderObjectElement**:
```dart
@override
void update(covariant RenderObjectWidget newWidget) {
  super.update(newWidget);
  _performRebuild();  // Calls widget.updateRenderObject()
}
```

---

## Part 5: Attachment - Linking RenderObjects

### RenderObjectElement.attachRenderObject()

```dart
// From widgets/framework.dart ~line 4430+
void attachRenderObject(Object? newSlot) {
  assert(_renderObject != null);
  assert(!attached);
  
  _slot = newSlot;
  
  // Find the parent RenderObjectElement
  final RenderObjectElement? parentRenderObjectElement = 
      _findAncestorRenderObjectElement();
  
  if (parentRenderObjectElement != null) {
    // ATTACH TO PARENT RENDER OBJECT
    parentRenderObjectElement.insertRenderObjectChild(_renderObject!, newSlot);
  }
  
  assert(attached);
}
```

**What happens**: The RenderObject is inserted into its parent's RenderObject child list.

### SingleChildRenderObjectElement.insertRenderObjectChild()

```dart
// From widgets/framework.dart ~line 7126
void insertRenderObjectChild(RenderObject child, Object? slot) {
  final renderObject = this.renderObject as RenderObjectWithChildMixin<RenderObject>;
  assert(slot == null);
  renderObject.child = child;  // ← SET THE CHILD RENDER OBJECT
}
```

### MultiChildRenderObjectElement.insertRenderObjectChild()

```dart
// (Implemented in RenderObject's ContainerRenderObjectMixin)
void insertRenderObjectChild(RenderObject child, Object? slot) {
  final IndexedSlot<RenderObject?> indexedSlot = slot as IndexedSlot<RenderObject?>;
  
  if (indexedSlot.value == null) {
    insertFirst(child);  // Insert at head
  } else {
    insertAfter(indexedSlot.value!, child);  // Insert in linked list
  }
}
```

---

## Part 6: Deactivation & Disposal Pipeline

### When an Element is Removed

```dart
// From widgets/framework.dart ~line 4460
void deactivateChild(Element child) {
  assert(child._parent == this);
  
  child._parent = null;
  child.detachRenderObject();  // ← DETACH FROM PARENT RENDEROBJECT
  
  owner!._inactiveElements.add(child);  // Move to inactive pool
}
```

**What happens**:
1. Parent link is severed
2. RenderObject is detached from parent's RenderObject tree
3. Element moves to "inactive" state (may be reused via GlobalKey)

### RenderObjectElement.detachRenderObject()

```dart
void detachRenderObject() {
  if (attached) {
    final RenderObjectElement? parentRenderObjectElement =
        _findAncestorRenderObjectElement();
    
    if (parentRenderObjectElement != null) {
      parentRenderObjectElement.removeRenderObjectChild(_renderObject!);
    }
  }
  _slot = null;
}
```

### Unmounting (Final Cleanup)

```dart
// From widgets/framework.dart ~line 4419
void unmount() {
  // Unregister global keys
  final Key? key = widget.key;
  if (key is GlobalKey) {
    owner!._unregisterGlobalKey(key, this);
  }
}

// StatefulElement unmount
@override
void unmount() {
  super.unmount();
  state.dispose();  // ← USER CLEANUP HAPPENS HERE
  state._element = null;
  _state = null;
}

// RenderObjectElement unmount
@override
void unmount() {
  super.unmount();
  oldWidget.didUnmountRenderObject(renderObject);
  _renderObject!.dispose();
  _renderObject = null;
}
```

---

## Part 7: Multi-Child Reconciliation (updateChildren)

```dart
// From widgets/framework.dart ~line 3685
List<Element> updateChildren(
  List<Element> oldChildren,
  List<Widget> newWidgets, {
  Set<Element>? forgottenChildren,
  List<Object?>? slots,
}) {
  // The algorithm:
  // 1. Match from start until first mismatch
  // 2. Match from end until first mismatch
  // 3. For the middle section, use Keys to match old→new
  // 4. Create new elements for unmatched new widgets
  // 5. Deactivate unmatched old elements
  
  var newChildrenTop = 0;
  var oldChildrenTop = 0;
  int newChildrenBottom = newWidgets.length - 1;
  int oldChildrenBottom = oldChildren.length - 1;
  
  final newChildren = List<Element>.filled(
    newWidgets.length,
    _NullElement.instance,
  );
  
  Element? previousChild;
  
  // Phase 1: Match from top
  while ((oldChildrenTop <= oldChildrenBottom) && 
         (newChildrenTop <= newChildrenBottom)) {
    final Element? oldChild = oldChildren[oldChildrenTop];
    final Widget newWidget = newWidgets[newChildrenTop];
    
    if (!Widget.canUpdate(oldChild.widget, newWidget)) {
      break;
    }
    
    final Element newChild = updateChild(
      oldChild,
      newWidget,
      slotFor(newChildrenTop, previousChild),
    )!;
    
    newChildren[newChildrenTop] = newChild;
    previousChild = newChild;
    newChildrenTop += 1;
    oldChildrenTop += 1;
  }
  
  // Phase 2: Match from bottom
  while ((oldChildrenTop <= oldChildrenBottom) && 
         (newChildrenTop <= newChildrenBottom)) {
    final Element? oldChild = oldChildren[oldChildrenBottom];
    final Widget newWidget = newWidgets[newChildrenBottom];
    
    if (!Widget.canUpdate(oldChild.widget, newWidget)) {
      break;
    }
    
    newChildrenBottom -= 1;
    oldChildrenBottom -= 1;
  }
  
  // Phase 3 & 4: Handle middle by keys
  Map<Key, Element>? keyedOldChildren;
  for (var i = oldChildrenTop; i <= oldChildrenBottom; i += 1) {
    final Element oldChild = oldChildren[i]!;
    final key = oldChild.widget.key;
    
    if (key != null) {
      keyedOldChildren ??= <Key, Element>{};
      keyedOldChildren[key] = oldChild;
    } else {
      deactivateChild(oldChild);
    }
  }
  
  for (var i = newChildrenTop; i <= newChildrenBottom; i += 1) {
    final Widget newWidget = newWidgets[i];
    final Key? key = newWidget.key;
    
    Element? oldChild;
    if (key != null) {
      oldChild = keyedOldChildren?[key];
      if (oldChild != null) {
        keyedOldChildren.remove(key);
      }
    }
    
    final Element newChild = updateChild(
      oldChild,
      newWidget,
      slotFor(i, previousChild),
    )!;
    
    newChildren[i] = newChild;
    previousChild = newChild;
  }
  
  // Phase 5: Deactivate remaining old children
  keyedOldChildren?.forEach((key, oldChild) {
    deactivateChild(oldChild);
  });
  
  return newChildren;
}
```

**Key insight**: The algorithm is O(n) when widgets maintain keys. Without keys, insertions/deletions in the middle cause O(n²) rebuilds.

---

## Part 8: Build Scope & Dependency Tracking

### InheritedWidget Dependency Tracking

```dart
// From widgets/framework.dart
void _updateInheritance() {
  _inheritedElements = _parent?._inheritedElements;
}

// When a widget calls dependOnInheritedWidgetOfExactType:
InheritedWidget dependOnInheritedWidgetOfExactType<T>() {
  final InheritedElement ancestor = 
      _findAncestorElementOfExactType<T>()!;
  
  _dependencies ??= HashSet<InheritedElement>();
  _dependencies!.add(ancestor);
  
  ancestor._dependents.add(this);
  
  return ancestor.widget as T;
}

// When InheritedWidget updates:
void notifyClients(InheritedWidget oldWidget) {
  for (final Element dependent in _dependents) {
    dependent._dependencies?.add(this);
    dependent.didChangeDependencies();  // Marks dirty
  }
}
```

**Key**: Dirty propagation only reaches elements that explicitly depend on the InheritedWidget.

---

## Summary: The Complete Lifecycle

```
┌─────────────────────────────────────────────────────────────┐
│                  USER CODE: build()                         │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                   Widget (Immutable)                        │
│           Configuration Object Describing UI                │
└─────────────────────────────────────────────────────────────┘
                            │
                    ┌───────┴───────┐
                    │               │
                    ▼               ▼
        ┌──────────────────┐  ┌──────────────────┐
        │ createElement()  │  │   canUpdate()    │
        │   (Inflates)     │  │   (Reconciles)   │
        └──────────────────┘  └──────────────────┘
                    │               │
                    └───────┬───────┘
                            ▼
┌─────────────────────────────────────────────────────────────┐
│          Element (Mutable, Lifecycle-aware)                 │
│  • Owns RenderObject                                        │
│  • Tracks State (if StatefulWidget)                         │
│  • Manages _dirty flag                                      │
│  • Controls build/rebuild pipeline                          │
└─────────────────────────────────────────────────────────────┘
                            │
                    ┌───────┴────────────┐
                    │                    │
                    ▼                    ▼
        ┌──────────────────┐  ┌──────────────────┐
        │ performRebuild() │  │ attachRenderObject()
        │ (Calls build())  │  │ (Links RenderObject)
        └──────────────────┘  └──────────────────┘
                    │                    │
                    └───────┬────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────┐
│             RenderObject (Actual Rendering)                 │
│  • layout() - Compute size and position                     │
│  • paint() - Draw to canvas                                 │
│  • Never knows about Widgets                                │
│  • Only knows parent RenderObject & Constraints             │
└─────────────────────────────────────────────────────────────┘
```

### Key Control Points

| Phase | Method | Who Controls | What Happens |
|-------|--------|-------------|--------------|
| **Initial** | `Widget.createElement()` | Element class | Creates appropriate Element subclass |
| **Mount** | `Element.mount()` | BuildOwner | Enters tree, establishes parent link, registers keys |
| **First Build** | `ComponentElement._firstBuild()` | BuildOwner | Triggers first `rebuild()` |
| **Rebuild** | `Element.rebuild()` | BuildOwner | Checks if dirty, calls `performRebuild()` |
| **Build** | `ComponentElement.performRebuild()` | Element | Calls user's `build()`, then `updateChild()` |
| **Update RO** | `RenderObjectElement.performRebuild()` | Element | Calls `widget.updateRenderObject()` |
| **Reconcile** | `Element.updateChild()` | Element | Decides: create/update/remove child Element |
| **Attach** | `RenderObjectElement.attachRenderObject()` | Element | Links RenderObject to parent RenderObject |
| **Layout** | `RenderObject.layout()` | PipelineOwner | Computes size and position |
| **Paint** | `RenderObject.paint()` | PipelineOwner | Draws to canvas |

---

## The BuildOwner: Master Controller

```dart
// From widgets/framework.dart ~line 2400
class BuildOwner {
  BuildScope _buildScope = BuildScope();
  final Map<GlobalKey, Element> _globalKeyRegistry = {};
  final Set<Element> _inactiveElements = {};
  
  void scheduleBuildFor(Element element) {
    element.buildScope._scheduleBuildFor(element);
    
    if (!_scheduledFlushDirtyElements && onBuildScheduled != null) {
      _scheduledFlushDirtyElements = true;
      onBuildScheduled!();
    }
  }
  
  void buildScope(Element context, [VoidCallback? callback]) {
    // Execute callback with state locked
    // Then flush all dirty elements in depth order
  }
  
  void finalizeTree() {
    // Unmount inactive elements
    // Verify global key constraints
  }
}
```

**Responsibilities**:
1. Maintains the dirty elements list
2. Prevents setState() during build via `_debugStateLocked`
3. Manages GlobalKey registry
4. Coordinates lifecycle transitions

The BuildOwner is the only object that can trigger rebuilds—not the Elements themselves.

---

## Performance Implications

### Expensive Operations
- **Creating new Element**: Allocates memory, registers global keys
- **Deactivating Element**: Moves to inactive pool (may be reused)
- **Unmounting Element**: Permanently removes, calls dispose()
- **Changing child count**: O(n²) without keys, O(n) with keys

### Cheap Operations
- **Same widget instance**: No rebuild, just slot update
- **Const widgets**: Skipped by identity check
- **Inherited widget change**: Only dependent subtrees marked dirty
- **RenderObject update**: Just property changes, no tree restructuring

### Optimization Strategies
1. Use `const` widgets to skip identity checks
2. Use `Key`s for dynamic lists (enables O(n) reconciliation)
3. Extract subtrees to avoid rebuilding unchanged parts
4. Use `shouldRebuild()` on InheritedWidget to control propagation

---

## Code References

All line numbers and code snippets reference Flutter's master branch:
- `/packages/flutter/lib/src/widgets/framework.dart` - Widget/Element control
- `/packages/flutter/lib/src/rendering/object.dart` - RenderObject system

This analysis is based on the actual Flutter source code, not documentation or specifications.
