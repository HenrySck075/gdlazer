#pragma once

#include <cmath>
#include <memory>
#include <vector>
#include <Geode/cocos/include/cocos2d.h>
#include <skia/include/core/SkCanvas.h>
#include "../../foundation/log.hpp"
#include "../../foundation/utils/macros.h"
#include "../../dartui/basic_types.hpp"
#include "gdlazer/caffeine/painting/edge_insets.hpp"

namespace caffeine {

// ============================================================================
// BoxConstraints: Describes valid size ranges
// ============================================================================

struct BoxConstraints {
  float minWidth = 0.0f;
  float maxWidth = INFINITY;
  float minHeight = 0.0f;
  float maxHeight = INFINITY;

  BoxConstraints() = default;
  BoxConstraints(float minW, float maxW, float minH, float maxH)
    : minWidth(minW), maxWidth(maxW), minHeight(minH), maxHeight(maxH) {}

  // Create tight constraints (exact size)
  static BoxConstraints tight(Size size) {
    auto width = size.width, height = size.height;
    return BoxConstraints(width, width, height, height);
  }

  // Create loose constraints (at most size)
  static BoxConstraints loose(Size maxSize) {
    return BoxConstraints(0, maxSize.width, 0, maxSize.height);
  }

  // Create expanding constraints (fill available space)
  static BoxConstraints expand(float width = INFINITY, float height = INFINITY) {
    return BoxConstraints(width, width, height, height);
  }

  bool isTight() const {
    return minWidth == maxWidth && minHeight == maxHeight;
  }

  bool hasTightWidth() const { return minWidth == maxWidth; }
  bool hasTightHeight() const { return minHeight == maxHeight; }

  bool hasBoundedWidth() const { return maxWidth != INFINITY; }
  bool hasBoundedHeight() const { return maxHeight != INFINITY; }

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

  BoxConstraints loosen() const {
    return {0,maxWidth,0,maxHeight};
  }

  // Reduce constraints by padding all sides
  BoxConstraints deflate(float padding) const {
    EdgeInsets m = EdgeInsets::all(padding);
    return deflate(&m);
  }

  // Reduce constraints by specific padding amounts
  BoxConstraints deflate(EdgeInsetsGeometry* insets) const {
    float horizontal = insets->horizontal();
    float vertical = insets->vertical();
    return BoxConstraints(
      std::max(0.0f, minWidth - horizontal),
      std::max(0.0f, maxWidth - horizontal),
      std::max(0.0f, minHeight - vertical),
      std::max(0.0f, maxHeight - vertical)
    );
  }

  // Returns new box constraints that respect the given constraints while being as close as possible to the original constraints.
  BoxConstraints enforce(const BoxConstraints& constraints) const {
    return BoxConstraints(
      std::max(minWidth, constraints.minWidth),
      std::min(maxWidth, constraints.maxWidth),
      std::max(minHeight, constraints.minHeight),
      std::min(maxHeight, constraints.maxHeight)
    );
  }

  bool operator==(const BoxConstraints& other) const {
    return minWidth == other.minWidth && maxWidth == other.maxWidth &&
           minHeight == other.minHeight && maxHeight == other.maxHeight;
  }
};

class ParentData {};

// ============================================================================
// RenderObject: Base class for layout
// ============================================================================

class RenderObject : public log::StringConvertible, public std::enable_shared_from_this<RenderObject> {
protected:
  BoxConstraints m_constraints;
  Size m_size;
  std::shared_ptr<RenderObject> m_parent;
  bool m_needsLayout = true;
  bool m_needsPaint = true;

public:
  virtual std::shared_ptr<ParentData> getParentData() const { return nullptr; }
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

class RenderBox : virtual public RenderObject {
public:
  struct BoxParentData : public ParentData {
    Offset offset;  // Position set by parent
  };

protected:
  std::shared_ptr<BoxParentData> m_parentData;

public:
  RenderBox() : m_parentData(std::make_shared<BoxParentData>()) {}
  virtual ~RenderBox() = default;

  std::shared_ptr<ParentData> getParentData() const { return m_parentData; } 

};

// ============================================================================
// RenderObjectWithChildMixin: Base for layouts with one child
// ============================================================================

class RenderObjectWithChildMixin : virtual public RenderObject {
protected:
  std::shared_ptr<RenderObject> m_child;

public:
  void setChild(std::shared_ptr<RenderObject> child) {
    m_child = child;
    if (child) {
      child->setParent(shared_from_this());
    }
    markNeedsLayout();
  }

  std::shared_ptr<RenderObject> getChild() const { return m_child; }
};

struct ChildLayoutHelper final {

  // Layout helper: call this in performLayout
  static Size layoutChild(std::shared_ptr<RenderBox> child,
                          const BoxConstraints &childConstraints);
  // Position helper for multi-child
  static void positionChild(std::shared_ptr<RenderBox> child,
                            const Offset &offset);
};

// ============================================================================
// ContainerRenderObjectMixin: Base for layouts with multiple children
// ============================================================================

class ContainerRenderObjectMixin : virtual public RenderObject {
protected:
  std::vector<std::shared_ptr<RenderBox>> m_children;

public:
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
};

}  // namespace caffeine

template<>
class fmt::formatter<caffeine::BoxConstraints> : public fmt::formatter<std::string> {
public:
  auto format(const caffeine::BoxConstraints& c, format_context& ctx) const {
    return fmt::format_to(ctx.out(), 
      "BoxConstraints(minW: {}, maxW: {}, minH: {}, maxH: {})",
      c.minWidth, c.maxWidth, c.minHeight, c.maxHeight);
  }
};
