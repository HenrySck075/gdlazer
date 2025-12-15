#pragma once

#include "gdlazer/caffeine/painting/basic_types.hpp"
#include <cmath>
#include <optional>
#include <memory>
#include <algorithm>

namespace caffeine {

enum class TextDirection {
  ltr,
  rtl,
};

// Forward declarations
struct EdgeInsets;
struct EdgeInsetsDirectional;

/// Base class for [EdgeInsets] that allows for text-direction aware resolution.
struct EdgeInsetsGeometry {
  EdgeInsetsGeometry() = default;
  virtual ~EdgeInsetsGeometry() = default;

  virtual double get_bottom() const = 0;
  virtual double get_end() const = 0;
  virtual double get_left() const = 0;
  virtual double get_right() const = 0;
  virtual double get_start() const = 0;
  virtual double get_top() const = 0;

  double _bottom() const { return get_bottom(); }
  double _end() const { return get_end(); }
  double _left() const { return get_left(); }
  double _right() const { return get_right(); }
  double _start() const { return get_start(); }
  double _top() const { return get_top(); }

  /// Whether every dimension is non-negative.
  virtual bool isNonNegative() const {
    return _left() >= 0.0 && _right() >= 0.0 && _start() >= 0.0 && 
           _end() >= 0.0 && _top() >= 0.0 && _bottom() >= 0.0;
  }

  /// The total offset in the horizontal direction.
  double horizontal() const {
    return _left() + _right() + _start() + _end();
  }

  /// The total offset in the vertical direction.
  double vertical() const {
    return _top() + _bottom();
  }

  /// The total offset in the given direction.
  double along(Axis axis) const {
    switch (axis) {
      case Axis::horizontal:
        return horizontal();
      case Axis::vertical:
        return vertical();
    }
    return 0.0;
  }

  /// The size that this [EdgeInsets] would occupy with an empty interior.
  Size collapsedSize() const {
    return Size(horizontal(), vertical());
  }

  /// An [EdgeInsetsGeometry] with top and bottom, left and right, and start and end flipped.
  virtual EdgeInsetsGeometry* flipped() const = 0;

  /// Returns a new size that is bigger than the given size.
  Size inflateSize(Size size) const {
    return Size(size.width + horizontal(), size.height + vertical());
  }

  /// Returns a new size that is smaller than the given size.
  Size deflateSize(Size size) const {
    return Size(size.width - horizontal(), size.height - vertical());
  }

  /// Returns the difference between two [EdgeInsetsGeometry] objects.
  virtual EdgeInsetsGeometry* subtract(const EdgeInsetsGeometry& other) const = 0;

  /// Returns the sum of two [EdgeInsetsGeometry] objects.
  virtual EdgeInsetsGeometry* add(const EdgeInsetsGeometry& other) const = 0;

  /// Returns a new [EdgeInsetsGeometry] object with all values clamped.
  virtual EdgeInsetsGeometry* clamp(const EdgeInsetsGeometry& min, const EdgeInsetsGeometry& max) const = 0;

  /// Returns the [EdgeInsetsGeometry] object with each dimension negated.
  virtual EdgeInsetsGeometry* operator-() const = 0;

  /// Scales the [EdgeInsetsGeometry] object in each dimension by the given factor.
  virtual EdgeInsetsGeometry* operator*(double other) const = 0;

  /// Divides the [EdgeInsetsGeometry] object in each dimension by the given factor.
  virtual EdgeInsetsGeometry* operator/(double other) const = 0;

  /// Integer divides the [EdgeInsetsGeometry] object in each dimension by the given factor.
  virtual EdgeInsetsGeometry* operator/(int other) const = 0;

  /// Computes the remainder in each dimension by the given factor.
  virtual EdgeInsetsGeometry* operator%(double other) const = 0;

  /// Linearly interpolate between two [EdgeInsetsGeometry] objects.
  static std::unique_ptr<EdgeInsetsGeometry> lerp(const EdgeInsetsGeometry& a, const EdgeInsetsGeometry& b, double t);

  /// Convert this instance into an [EdgeInsets].
  virtual EdgeInsets* resolve(TextDirection direction) const = 0;
};

/// An immutable set of offsets in each of the four cardinal directions.
struct EdgeInsets : public EdgeInsetsGeometry {
  double left;
  double top;
  double right;
  double bottom;

  /// Creates insets from offsets from the left, top, right, and bottom.
  constexpr EdgeInsets(double left = 0.0, double top = 0.0, 
                       double right = 0.0, double bottom = 0.0)
    : left(left), top(top), right(right), bottom(bottom) {}

  /// Creates insets where all the offsets are `value`.
  static constexpr EdgeInsets all(double value) {
    return EdgeInsets(value, value, value, value);
  }

  /// Creates insets with only the given values non-zero.
  static constexpr EdgeInsets only(double left = 0.0, double top = 0.0, 
                                    double right = 0.0, double bottom = 0.0) {
    return EdgeInsets(left, top, right, bottom);
  }

  /// Creates insets with symmetrical vertical and horizontal offsets.
  static constexpr EdgeInsets symmetric(double vertical = 0.0, double horizontal = 0.0) {
    return EdgeInsets(horizontal, vertical, horizontal, vertical);
  }

  /// An [EdgeInsets] with zero offsets in each direction.
  //static constexpr EdgeInsets zero = EdgeInsets(0.0, 0.0, 0.0, 0.0);

  double get_bottom() const override { return bottom; }
  double get_top() const override { return top; }
  double get_left() const override { return left; }
  double get_right() const override { return right; }
  double get_start() const override { return 0.0; }
  double get_end() const override { return 0.0; }

  /// An Offset describing the vector from the top left of a rectangle.
  Offset topLeft() const { return Offset(left, top); }

  /// An Offset describing the vector from the top right of a rectangle.
  Offset topRight() const { return Offset(-right, top); }

  /// An Offset describing the vector from the bottom left of a rectangle.
  Offset bottomLeft() const { return Offset(left, -bottom); }

  /// An Offset describing the vector from the bottom right of a rectangle.
  Offset bottomRight() const { return Offset(-right, -bottom); }

  /// An [EdgeInsets] with top and bottom as well as left and right flipped.
  EdgeInsets* flipped() const override {
    return new EdgeInsets(right, bottom, left, top);
  }

  /// Returns a new rect that is bigger than the given rect.
  Rect inflateRect(Rect rect) const {
    return Rect(rect.left - left, rect.top - top, 
                          rect.right + right, rect.bottom + bottom);
  }

  EdgeInsetsGeometry* subtract(const EdgeInsetsGeometry& other) const override {
    if (auto* casted = dynamic_cast<const EdgeInsets*>(&other)) {
      return new EdgeInsets(left - casted->left, top - casted->top,
                            right - casted->right, bottom - casted->bottom);
    }
    return new EdgeInsets(*this);
  }

  EdgeInsetsGeometry* add(const EdgeInsetsGeometry& other) const override {
    if (auto* casted = dynamic_cast<const EdgeInsets*>(&other)) {
      return new EdgeInsets(left + casted->left, top + casted->top,
                            right + casted->right, bottom + casted->bottom);
    }
    return new EdgeInsets(*this);
  }

  EdgeInsetsGeometry* clamp(const EdgeInsetsGeometry& min, const EdgeInsetsGeometry& max) const override {
    if (auto* minCasted = dynamic_cast<const EdgeInsets*>(&min)) {
      if (auto* maxCasted = dynamic_cast<const EdgeInsets*>(&max)) {
        return new EdgeInsets(
          std::max(minCasted->left, std::min(left, maxCasted->left)),
          std::max(minCasted->top, std::min(top, maxCasted->top)),
          std::max(minCasted->right, std::min(right, maxCasted->right)),
          std::max(minCasted->bottom, std::min(bottom, maxCasted->bottom))
        );
      }
    }
    return new EdgeInsets(*this);
  }

  /// Returns the difference between two [EdgeInsets].
  EdgeInsets operator-(const EdgeInsets& other) const {
    return EdgeInsets(left - other.left, top - other.top,
                      right - other.right, bottom - other.bottom);
  }

  /// Returns the sum of two [EdgeInsets].
  EdgeInsets operator+(const EdgeInsets& other) const {
    return EdgeInsets(left + other.left, top + other.top,
                      right + other.right, bottom + other.bottom);
  }

  /// Returns the [EdgeInsets] object with each dimension negated.
  EdgeInsets* operator-() const override {
    return new EdgeInsets(-left, -top, -right, -bottom);
  }

  /// Scales the [EdgeInsets] in each dimension by the given factor.
  EdgeInsets* operator*(double other) const override {
    return new EdgeInsets(left * other, top * other, right * other, bottom * other);
  }

  /// Divides the [EdgeInsets] in each dimension by the given factor.
  EdgeInsets* operator/(double other) const override {
    return new EdgeInsets(left / other, top / other, right / other, bottom / other);
  }

  /// Integer divides the [EdgeInsets] in each dimension by the given factor.
  EdgeInsets* operator/(int other) const override {
    return new EdgeInsets(std::floor(left / other), std::floor(top / other),
                          std::floor(right / other), std::floor(bottom / other));
  }

  /// Computes the remainder in each dimension by the given factor.
  EdgeInsets* operator%(double other) const override {
    return new EdgeInsets(std::fmod(left, other), std::fmod(top, other),
                          std::fmod(right, other), std::fmod(bottom, other));
  }

  /// Linearly interpolate between two [EdgeInsets].
  static std::unique_ptr<EdgeInsets> lerp(const EdgeInsets& a, const EdgeInsets& b, double t);

  /// Convert this instance into an [EdgeInsets].
  EdgeInsets* resolve(TextDirection direction) const override {
    return new EdgeInsets(*this);
  }

  /// Creates a copy of this EdgeInsets with the given fields replaced.
  EdgeInsets copyWith(const std::optional<double>& left = std::nullopt,
                      const std::optional<double>& top = std::nullopt,
                      const std::optional<double>& right = std::nullopt,
                      const std::optional<double>& bottom = std::nullopt) const {
    return EdgeInsets(left.value_or(this->left),
                      top.value_or(this->top),
                      right.value_or(this->right),
                      bottom.value_or(this->bottom));
  }
};

/// An immutable set of offsets in each of the four cardinal directions, but
/// whose horizontal components are dependent on the writing direction.
struct EdgeInsetsDirectional : public EdgeInsetsGeometry {
  double start;
  double top;
  double end;
  double bottom;

  /// Creates insets from offsets from the start, top, end, and bottom.
  constexpr EdgeInsetsDirectional(double start = 0.0, double top = 0.0, 
                                  double end = 0.0, double bottom = 0.0)
    : start(start), top(top), end(end), bottom(bottom) {}

  /// Creates insets with only the given values non-zero.
  static constexpr EdgeInsetsDirectional only(double start = 0.0, double top = 0.0,
                                               double end = 0.0, double bottom = 0.0) {
    return EdgeInsetsDirectional(start, top, end, bottom);
  }

  /// Creates insets with symmetric vertical and horizontal offsets.
  static constexpr EdgeInsetsDirectional symmetric(double vertical = 0.0, double horizontal = 0.0) {
    return EdgeInsetsDirectional(horizontal, vertical, horizontal, vertical);
  }

  /// Creates insets where all the offsets are `value`.
  static constexpr EdgeInsetsDirectional all(double value) {
    return EdgeInsetsDirectional(value, value, value, value);
  }

  /// An [EdgeInsetsDirectional] with zero offsets in each direction.
  //static constexpr EdgeInsetsDirectional zero = EdgeInsetsDirectional(0.0, 0.0, 0.0, 0.0);

  double get_start() const override { return start; }
  double get_top() const override { return top; }
  double get_end() const override { return end; }
  double get_bottom() const override { return bottom; }
  double get_left() const override { return 0.0; }
  double get_right() const override { return 0.0; }

  bool isNonNegative() const override {
    return start >= 0.0 && top >= 0.0 && end >= 0.0 && bottom >= 0.0;
  }

  /// An [EdgeInsetsDirectional] with start and end as well as top and bottom flipped.
  EdgeInsetsDirectional* flipped() const override {
    return new EdgeInsetsDirectional(end, bottom, start, top);
  }

  EdgeInsetsGeometry* subtract(const EdgeInsetsGeometry& other) const override {
    if (auto* casted = dynamic_cast<const EdgeInsetsDirectional*>(&other)) {
      return new EdgeInsetsDirectional(start - casted->start, top - casted->top,
                                       end - casted->end, bottom - casted->bottom);
    }
    return new EdgeInsetsDirectional(*this);
  }

  EdgeInsetsGeometry* add(const EdgeInsetsGeometry& other) const override {
    if (auto* casted = dynamic_cast<const EdgeInsetsDirectional*>(&other)) {
      return new EdgeInsetsDirectional(start + casted->start, top + casted->top,
                                       end + casted->end, bottom + casted->bottom);
    }
    return new EdgeInsetsDirectional(*this);
  }

  EdgeInsetsGeometry* clamp(const EdgeInsetsGeometry& min, const EdgeInsetsGeometry& max) const override {
    if (auto* minCasted = dynamic_cast<const EdgeInsetsDirectional*>(&min)) {
      if (auto* maxCasted = dynamic_cast<const EdgeInsetsDirectional*>(&max)) {
        return new EdgeInsetsDirectional(
          std::max(minCasted->start, std::min(start, maxCasted->start)),
          std::max(minCasted->top, std::min(top, maxCasted->top)),
          std::max(minCasted->end, std::min(end, maxCasted->end)),
          std::max(minCasted->bottom, std::min(bottom, maxCasted->bottom))
        );
      }
    }
    return new EdgeInsetsDirectional(*this);
  }

  /// Returns the difference between two [EdgeInsetsDirectional] objects.
  EdgeInsetsDirectional operator-(const EdgeInsetsDirectional& other) const {
    return EdgeInsetsDirectional(start - other.start, top - other.top,
                                 end - other.end, bottom - other.bottom);
  }

  /// Returns the sum of two [EdgeInsetsDirectional] objects.
  EdgeInsetsDirectional operator+(const EdgeInsetsDirectional& other) const {
    return EdgeInsetsDirectional(start + other.start, top + other.top,
                                 end + other.end, bottom + other.bottom);
  }

  /// Returns the [EdgeInsetsDirectional] object with each dimension negated.
  EdgeInsetsDirectional* operator-() const override {
    return new EdgeInsetsDirectional(-start, -top, -end, -bottom);
  }

  /// Scales the [EdgeInsetsDirectional] in each dimension by the given factor.
  EdgeInsetsDirectional* operator*(double other) const override {
    return new EdgeInsetsDirectional(start * other, top * other,
                                     end * other, bottom * other);
  }

  /// Divides the [EdgeInsetsDirectional] in each dimension by the given factor.
  EdgeInsetsDirectional* operator/(double other) const override {
    return new EdgeInsetsDirectional(start / other, top / other,
                                     end / other, bottom / other);
  }

  /// Integer divides the [EdgeInsetsDirectional] in each dimension by the given factor.
  EdgeInsetsDirectional* operator/(int other) const override {
    return new EdgeInsetsDirectional(start / (int)other, top / (int)other,
                                     end / (int)other, bottom / (int)other);
  }

  /// Computes the remainder in each dimension by the given factor.
  EdgeInsetsDirectional* operator%(double other) const override {
    return new EdgeInsetsDirectional(std::fmod(start, other), std::fmod(top, other),
                                     std::fmod(end, other), std::fmod(bottom, other));
  }

  /// Linearly interpolate between two [EdgeInsetsDirectional].
  static std::unique_ptr<EdgeInsetsDirectional> lerp(const EdgeInsetsDirectional& a, 
                                                      const EdgeInsetsDirectional& b, double t);

  /// Convert this instance into an [EdgeInsets].
  EdgeInsets* resolve(TextDirection direction) const override;

  /// Creates a copy of this EdgeInsetsDirectional with the given fields replaced.
  EdgeInsetsDirectional copyWith(const std::optional<double>& start = std::nullopt,
                                 const std::optional<double>& top = std::nullopt,
                                 const std::optional<double>& end = std::nullopt,
                                 const std::optional<double>& bottom = std::nullopt) const {
    return EdgeInsetsDirectional(start.value_or(this->start),
                                 top.value_or(this->top),
                                 end.value_or(this->end),
                                 bottom.value_or(this->bottom));
  }
};

} // namespace caffeine
