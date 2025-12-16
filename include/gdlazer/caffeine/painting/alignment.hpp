#pragma once

#include "../dartui/basic_types.hpp"
#include <cmath>

namespace caffeine {

/// Base class for specifying an alignment within a rectangle.
/// AlignmentGeometry is an abstract class; Alignment is a concrete subclass.
struct AlignmentGeometry {
  AlignmentGeometry() = default;
  virtual ~AlignmentGeometry() = default;

  /// The offset of the child's leading edge from the leading edge of the parent
  /// in the horizontal direction.
  virtual double getX() const = 0;

  /// The offset of the child's leading edge from the leading edge of the parent
  /// in the vertical direction.
  virtual double getY() const = 0;

  /// Create an [Offset] describing the location of the child's top-left corner
  /// within a box of the given size.
  ///
  /// For example, [Alignment.topLeft] at size 200,100 would be an offset
  /// of (0, 0), and [Alignment.bottomRight] at size 200,100 would be an offset
  /// of (200, 100).
  Offset alongSize(Size size) const {
    float cx = size.width / 2;
    float cy = size.height / 2;
    return Offset(cx + getX() * cx, cy + getY() * cy);
  }

  /// Create an [Offset] describing the location of the child's top-left corner
  /// within a box of the given size. The parent is expected to have padding.
  ///
  /// For example, [Alignment.topLeft] would return the offset (0, 0) and
  /// [Alignment.bottomRight] would return the offset (size.width, size.height).
  Offset alongOffset(Offset offset) const {
    float cx = offset.dx / 2;
    float cy = offset.dy / 2;
    return Offset(cx + getX() * cx, cy + getY() * cy);
  }

  virtual AlignmentGeometry* operator-() const = 0;
};

/// An alignment along both the horizontal and vertical axes.
///
/// Alignment.x is the position along the horizontal axis:
///  * -1.0 means left aligned
///  *  0.0 means horizontally centered
///  *  1.0 means right aligned
///
/// Alignment.y is the position along the vertical axis:
///  * -1.0 means top aligned
///  *  0.0 means vertically centered
///  *  1.0 means bottom aligned
struct Alignment : public AlignmentGeometry {
  float x;
  float y;

  Alignment() : x(0.0f), y(0.0f) {}
  Alignment(float x, float y) : x(x), y(y) {}

  double getX() const override { return static_cast<double>(x); }
  double getY() const override { return static_cast<double>(y); }

  // Common alignments as static const instances
  static const Alignment topLeft;
  static const Alignment topCenter;
  static const Alignment topRight;
  
  static const Alignment centerLeft;
  static const Alignment center;
  static const Alignment centerRight;
  
  static const Alignment bottomLeft;
  static const Alignment bottomCenter;
  static const Alignment bottomRight;

  Alignment* operator-() const override {
    return new Alignment(-x, -y);
  }

  bool operator==(const Alignment& other) const {
    return x == other.x && y == other.y;
  }
};

}  // namespace caffeine
