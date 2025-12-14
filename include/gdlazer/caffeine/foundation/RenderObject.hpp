#pragma once

#include <memory>
#include <vector>
#include <Geode/cocos/include/cocos2d.h>
#include <skia/include/core/SkCanvas.h>
#include "log.hpp"

namespace caffeine {

// ============================================================================
// BoxConstraints: Describes valid size ranges
// ============================================================================

struct Size {
  float width = 0.0f;
  float height = 0.0f;

  Size() = default;
  Size(float w, float h) : width(w), height(h) {}

  Size operator+(const Size& other) const {
    return Size(width + other.width, height + other.height);
  }

  Size operator-(const Size& other) const {
    return Size(width - other.width, height - other.height);
  }

  Size operator*(float scalar) const {
    return Size(width * scalar, height * scalar);
  }

  bool operator==(const Size& other) const {
    return width == other.width && height == other.height;
  }
};

struct Offset {
  float dx = 0.0f;
  float dy = 0.0f;

  Offset() = default;
  Offset(float x, float y) : dx(x), dy(y) {}

  Offset operator+(const Offset& other) const {
    return Offset(dx + other.dx, dy + other.dy);
  }

  Offset operator*(float scalar) const {
    return Offset(dx * scalar, dy * scalar);
  }

  bool operator==(const Offset& other) const {
    return dx == other.dx && dy == other.dy;
  }
};

struct Rect {
  Offset position;
  Size size;

  Rect() = default;
  Rect(Offset pos, Size sz) : position(pos), size(sz) {}

  static Rect fromLTWH(float left, float top, float width, float height) {
    return Rect(Offset(left, top), Size(width, height));
  }
};

constexpr float infinity = 1e10f;  // Large number representing infinity

struct BoxConstraints {
  float minWidth = 0.0f;
  float maxWidth = infinity;
  float minHeight = 0.0f;
  float maxHeight = infinity;

  BoxConstraints() = default;
  BoxConstraints(float minW, float maxW, float minH, float maxH)
    : minWidth(minW), maxWidth(maxW), minHeight(minH), maxHeight(maxH) {}

  // Create tight constraints (exact size)
  static BoxConstraints tight(float width, float height) {
    return BoxConstraints(width, width, height, height);
  }

  // Create loose constraints (at most size)
  static BoxConstraints loose(float maxW, float maxH) {
    return BoxConstraints(0, maxW, 0, maxH);
  }

  // Create expanding constraints (fill available space)
  static BoxConstraints expand(float width = infinity, float height = infinity) {
    return BoxConstraints(width, width, height, height);
  }

  bool isTight() const {
    return minWidth == maxWidth && minHeight == maxHeight;
  }

  bool hasTightWidth() const { return minWidth == maxWidth; }
  bool hasTightHeight() const { return minHeight == maxHeight; }

  bool hasBoundedWidth() const { return maxWidth != infinity; }
  bool hasBoundedHeight() const { return maxHeight != infinity; }

  Size biggest() const {
    return Size(maxWidth, maxHeight);
  }

  Size smallest() const {
    return Size(minWidth, minHeight);
  }

  Size constrain(const Size& size) const {
    return Size(
      std::max(minWidth, std::min(maxWidth, size.width)),
      std::max(minHeight, std::min(maxHeight, size.height))
    );
  }

  // Reduce constraints by padding all sides
  BoxConstraints deflate(float padding) const {
    return deflate(padding, padding, padding, padding);
  }

  // Reduce constraints by specific padding amounts
  BoxConstraints deflate(float left, float top, float right, float bottom) const {
    float horizontal = left + right;
    float vertical = top + bottom;
    return BoxConstraints(
      std::max(0.0f, minWidth - horizontal),
      std::max(0.0f, maxWidth - horizontal),
      std::max(0.0f, minHeight - vertical),
      std::max(0.0f, maxHeight - vertical)
    );
  }

  bool operator==(const BoxConstraints& other) const {
    return minWidth == other.minWidth && maxWidth == other.maxWidth &&
           minHeight == other.minHeight && maxHeight == other.maxHeight;
  }
};

// ============================================================================
// RenderObject: Base class for layout
// ============================================================================

class RenderObject : public log::WithLogger, public std::enable_shared_from_this<RenderObject> {
protected:
  BoxConstraints m_constraints;
  Size m_size;
  std::shared_ptr<RenderObject> m_parent;
  bool m_needsLayout = true;
  bool m_needsPaint = true;

public:
  RenderObject() = default;
  virtual ~RenderObject() = default;

  // Getters
  const BoxConstraints& getConstraints() const { return m_constraints; }
  const Size& getSize() const { return m_size; }
  std::shared_ptr<RenderObject> getParent() const { return m_parent; }
  bool needsLayout() const { return m_needsLayout; }
  bool needsPaint() const { return m_needsPaint; }

  // Main layout entry point
  void layout(const BoxConstraints& constraints, bool parentUsesSize = false);

  // Override this to implement layout logic
  virtual void performLayout() = 0;

  // Override this to implement painting
  // Paint to the Skia canvas
  virtual void paint(SkCanvas* canvas) = 0;

  // Mark layout as dirty
  void markNeedsLayout() {
    if (!m_needsLayout) {
      m_needsLayout = true;
      if (m_parent) {
        m_parent->markNeedsLayout();
      }
    }
  }

  // Mark paint as dirty
  void markNeedsPaint() {
    m_needsPaint = true;
  }

  // Set parent (internal use)
  void setParent(std::shared_ptr<RenderObject> parent) {
    m_parent = parent;
  }
};

// ============================================================================
// RenderBox: Base class for rectangular layouts
// ============================================================================

class RenderBox : public RenderObject {
public:
  struct BoxParentData {
    Offset offset;  // Position set by parent
  };

protected:
  std::shared_ptr<BoxParentData> m_parentData;

public:
  RenderBox() : m_parentData(std::make_shared<BoxParentData>()) {}
  virtual ~RenderBox() = default;

  std::shared_ptr<BoxParentData> getParentData() const { return m_parentData; }
};

// ============================================================================
// SingleChildRenderBox: Base for layouts with one child
// ============================================================================

class SingleChildRenderBox : public RenderBox {
protected:
  std::shared_ptr<RenderBox> m_child;

public:
  SingleChildRenderBox() = default;
  virtual ~SingleChildRenderBox() = default;

  void setChild(std::shared_ptr<RenderBox> child) {
    m_child = child;
    if (child) {
      child->setParent(shared_from_this());
    }
    markNeedsLayout();
  }

  std::shared_ptr<RenderBox> getChild() const { return m_child; }

  // Layout helper: call this in performLayout
  void layoutChild(const BoxConstraints& childConstraints, bool parentUsesSize = false) {
    if (m_child) {
      m_child->layout(childConstraints, parentUsesSize);
    }
  }

  // Position helper: call this after getting child size
  void positionChild(const Offset& offset) {
    if (m_child) {
      m_child->getParentData()->offset = offset;
    }
  }
};

// ============================================================================
// MultiChildRenderBox: Base for layouts with multiple children
// ============================================================================

class MultiChildRenderBox : public RenderBox {
protected:
  std::vector<std::shared_ptr<RenderBox>> m_children;

public:
  MultiChildRenderBox() = default;
  virtual ~MultiChildRenderBox() = default;

  void addChild(std::shared_ptr<RenderBox> child) {
    m_children.push_back(child);
    if (child) {
      child->setParent(shared_from_this());
    }
    markNeedsLayout();
  }

  void removeChild(std::shared_ptr<RenderBox> child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
      m_children.erase(it);
      markNeedsLayout();
    }
  }

  const std::vector<std::shared_ptr<RenderBox>>& getChildren() const {
    return m_children;
  }

  size_t getChildCount() const { return m_children.size(); }

  // Position helper for multi-child
  void positionChild(std::shared_ptr<RenderBox> child, const Offset& offset) {
    if (child) {
      child->getParentData()->offset = offset;
    }
  }
};

}  // namespace caffeine



// Models formatting (with fmt)
template<>
class fmt::formatter<caffeine::BoxConstraints> : public fmt::formatter<std::string> {
public:
  auto format(const caffeine::BoxConstraints& c, format_context& ctx) const {
    return fmt::format_to(ctx.out(), 
      "BoxConstraints(minW: {}, maxW: {}, minH: {}, maxH: {})",
      c.minWidth, c.maxWidth, c.minHeight, c.maxHeight);
  }
};

template<>
class fmt::formatter<caffeine::Size> : public fmt::formatter<std::string> {
public:
  auto format(const caffeine::Size& s, format_context& ctx) const {
    return fmt::format_to(ctx.out(), "Size({}, {})", s.width, s.height);
  }
};

template<>
class fmt::formatter<caffeine::Offset> : public fmt::formatter<std::string> {
public:
  auto format(const caffeine::Offset& o, format_context& ctx) const {
    return fmt::format_to(ctx.out(), "Offset({}, {})", o.dx, o.dy);
  }
};

template<>
class fmt::formatter<caffeine::Rect> : public fmt::formatter<std::string> {
public:
  auto format(const caffeine::Rect& r, format_context& ctx) const {
    return fmt::format_to(ctx.out(), "Rect(pos: ({}, {}), size: ({}, {}))",
      r.position.dx, r.position.dy, r.size.width, r.size.height);
  }
};