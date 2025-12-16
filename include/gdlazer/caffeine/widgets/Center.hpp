#pragma once

#include <memory>
#include "Align.hpp"
#include "../painting/alignment.hpp"
#include "../widgets/framework/Widget.hpp"

namespace caffeine {

// ============================================================================
// Center: Centers child within available space
// ============================================================================

/// A widget that centers its child within itself.
///
/// This is a convenience widget for aligning a child with Alignment::center.
class Center : public Align {
public:
  Center(Widget* child = nullptr)
    : Align(std::make_shared<Alignment>(Alignment::center), child, 1.0f, 1.0f) {}

  virtual ~Center() = default;
};

}  // namespace caffeine
