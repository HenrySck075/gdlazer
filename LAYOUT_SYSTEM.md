# Flutter Layout System (Constraints Down, Sizes Up, Positions Set by Parent)
**A Catgirl's Guide to Implementing Layout in Your C++ Port** ~ 🐱✨

---

## The Three-Phase Layout Algorithm

Flutter's rendering pipeline consists of three phases:

### Phase 1: **CONSTRAINTS FLOW DOWN** ⬇️
Parent tells child: "here's the space you can use"

```
Parent RenderObject
    |
    | layout(BoxConstraints)
    |
    v
Child RenderObject
    "I can be between 100-500px wide, and 50-300px tall"
```

### Phase 2: **SIZES FLOW UP** ⬆️
Child computes and reports: "I'm this big"

```
Child RenderObject
    |
    | this.size = computed_size
    |
    v
Parent RenderObject
    "Okay, child is 250px wide and 150px tall"
```

### Phase 3: **POSITIONS SET BY PARENT** ↗️
Parent tells child: "you go here"

```
Parent RenderObject
    |
    | child.parentData.offset = Offset(x, y)
    |
    v
Child RenderObject
    "I'm drawn at position (50, 100)"
```

---

## Core Concepts in C++

### 1. **BoxConstraints** (describes valid size ranges)

```cpp
struct BoxConstraints {
  float minWidth, maxWidth;
  float minHeight, maxHeight;

  // Helper queries
  bool isTight() const {
    return minWidth == maxWidth && minHeight == maxHeight;
  }

  bool hasBoundedWidth() const { return maxWidth != infinity; }
  bool hasBoundedHeight() const { return maxHeight != infinity; }
};
```

### 2. **RenderObject Base Class** (the layouting abstraction)

```cpp
class RenderObject {
protected:
  BoxConstraints m_constraints;
  Size m_size;

public:
  // Phase 1: Parent calls this with constraints
  virtual void layout(BoxConstraints constraints, bool parentUsesSize = false) {
    m_constraints = constraints;
    if (sizedByParent()) {
      m_size = computeDryLayout(constraints);
    }
    performLayout();  // Phase 2: Child computes size
    markNeedsPaint();
  }

  // Phase 2: Child computes its size based on constraints
  virtual void performLayout() = 0;

  // Phase 3 (set by parent after getting size):
  // parentData()->offset = Offset(x, y);
};
```

### 3. **RenderBox** (for rectangular layouts)

```cpp
class RenderBox : public RenderObject {
protected:
  struct BoxParentData {
    Offset offset;  // Position set by parent
    // Other parent-specific data
  };

  virtual void performLayout() = 0;

  // Called by parent to position this child
  void setChildPositionAndSize(RenderBox* child, Offset offset) {
    auto parentData = dynamic_cast<BoxParentData*>(child->parentData);
    parentData->offset = offset;
  }
};
```

---

## The Layout Flow in Practice

### Example: Container (has one child, adds padding)

```
┌─────────────────────────────────────┐
│  Parent (yellow background)         │
│  ┌───────────────────────────────┐  │
│  │   Padding (16px all sides)    │  │
│  │  ┌─────────────────────────┐  │  │
│  │  │  Child (blue box)       │  │  │
│  │  │  (should fill space)    │  │  │
│  │  └─────────────────────────┘  │  │
│  └───────────────────────────────┘  │
└─────────────────────────────────────┘
```

#### Step 1: Parent Calls Layout on Padding

```cpp
class PaddingRenderObject : public RenderBox {
  RenderBox* child;

  void performLayout() override {
    // Step 1: Remove padding from constraints
    BoxConstraints childConstraints = m_constraints.deflate(padding);

    // Step 2: Tell child to layout with reduced constraints
    child->layout(childConstraints, parentUsesSize = true);

    // Step 3: Get child's size and set our own size
    m_size = Size(
      child->m_size.width + padding.left + padding.right,
      child->m_size.height + padding.top + padding.bottom
    );

    // Step 4: Position the child (add padding offset)
    auto childParentData = dynamic_cast<BoxParentData*>(child->parentData);
    childParentData->offset = Offset(padding.left, padding.top);
  }
};
```

---

## Key Design Principles

### ✅ Constraints Are Immutable During Layout
Once a constraint is passed to `layout()`, the child cannot change it. This allows optimization.

### ✅ Size Is Computed Only Once
During `performLayout()`, the RenderObject sets `this->size` once and only once.

### ✅ Parent Controls Position
The child never positions itself. Only the parent can set `child->parentData->offset`.

### ✅ parentUsesSize Flag
If a child's size affects the parent's layout, parent must set `parentUsesSize=true` when calling `child->layout()`. This prevents unnecessary re-layouts.

```cpp
// BAD: might miss layout updates
child->layout(constraints);

// GOOD: if we use child's size
child->layout(constraints, true);  // parentUsesSize = true
```

### ✅ Tight vs Loose Constraints
- **Tight**: `minWidth == maxWidth && minHeight == maxHeight` → child must be this exact size
- **Loose**: `minWidth < maxWidth || minHeight < maxHeight` → child has options

```cpp
if (constraints.isTight()) {
  // Optimization: child size is determined, no need to check child layout
  m_size = constraints.biggest();
} else {
  // Child can choose size within constraints
  child->layout(constraints, parentUsesSize = true);
  m_size = child->m_size;
}
```

---

## Implementation Checklist for Your Port

- [ ] Create `BoxConstraints` struct with min/max width/height
- [ ] Create `RenderObject` base class with `layout(constraints)` method
- [ ] Create `RenderBox` extending `RenderObject` with position/size handling
- [ ] Implement `BoxParentData` for storing child offset
- [ ] Create `SingleChildRenderBox` base class (handles one child)
- [ ] Create `MultiChildRenderBox` base class (handles multiple children)
- [ ] Implement layout helpers:
  - [ ] `layoutChild(child, constraints, parentUsesSize)`
  - [ ] `positionChild(child, offset)`
  - [ ] `constraintsThatReducePadding(padding)`

---

## Example: Simple Center Widget

```cpp
class CenterRenderBox : public RenderBox {
  RenderBox* child;

  void performLayout() override {
    // Step 1: Give child the same constraints (no restriction)
    child->layout(m_constraints, parentUsesSize = true);

    // Step 2: We're same size as our constraints allow
    m_size = m_constraints.constrain(child->m_size);

    // Step 3: Position child in center
    Offset centerOffset = (m_size - child->m_size) * 0.5f;
    auto parentData = dynamic_cast<BoxParentData*>(child->parentData);
    parentData->offset = centerOffset;
  }
};
```

---

## Rendering (After Layout)

Once layout is done, rendering is simple:

```cpp
void paint(PaintContext ctx) {
  // Paint background
  ctx.paintRect(Rect::fromLTWH(0, 0, m_size.width, m_size.height), bgPaint);

  // Paint child at its offset
  ctx.translate(childParentData.offset);
  child->paint(ctx);
  ctx.restore();
}
```

---

## Why This Design Works So Well

| Benefit | How |
|---------|-----|
| **Predictable** | Child can only say "yes" or "no" to constraints, never negotiates |
| **Efficient** | Tight constraints allow skipping child layout checks |
| **Composable** | Any RenderBox can wrap any other RenderBox |
| **Cache-friendly** | Constraints passed down can be checked against previous values |
| **GPU-friendly** | Exact sizes known before rendering → no reflows |

---

## Key Files in Flutter Source

For reference, check out:
- `packages/flutter/lib/src/rendering/object.dart` ~ RenderObject base
- `packages/flutter/lib/src/rendering/box.dart` ~ RenderBox
- `packages/flutter/lib/src/rendering/proxy_box.dart` ~ SingleChildRenderBox
- `packages/flutter/lib/src/rendering/flex.dart` ~ Row/Column (multi-child)

---

*~ now go forth and implement constraints flow~ 🐱✨ ~*
