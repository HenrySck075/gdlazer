#pragma once
struct Vector2;
Vector2 operator*(float scalar, Vector2 lovely);
struct Vector2 {
  float x;
  float y;

  Vector2(float value) : x(value), y(value) {};
  Vector2(float x, float y) : x(x), y(y) {};

  float operator[](int idx) {
    if (idx == 0) return x;
    if (idx == 1) return y;
    throw std::out_of_range("Index out of range for Vector2");
  }

  inline float length() {
    return sqrtf(lengthS());
  };

  inline float lengthS() {
    return x*x + y*y;
  }

  Vector2 perpendicularRight() {
    return {y, -x};
  }
  Vector2 perpendicularLeft() {
    return {-y, x};
  }

  void normalize() {
    float scale = 1 / length();
    x *= scale;
    y *= scale;
  }
  Vector2 toNormalized() {
    Vector2 v = *this;
    v.normalize();
    return v;
  }

  /// Defines a unit-length Vector2 that points towards the X-axis.
  static const Vector2 UnitX;

  /// Defines a unit-length Vector2 that points towards the Y-axis.
  static const Vector2 UnitY;

  /// Defines a zero-length Vector2.
  static const Vector2 Zero;

  /// Defines an instance with all components set to 1.
  static const Vector2 One;

  Vector2 operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
  }
  Vector2 operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
  }
  Vector2 operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
  }
  Vector2 operator*(const Vector2& other) const {
    return Vector2(x * other.x, y * other.y);
  }
  Vector2 operator/(float scalar) const {
    if (scalar == 0) throw std::invalid_argument("Division by zero in Vector2 division");
    return Vector2(x / scalar, y / scalar);
  }
  Vector2 operator/(const Vector2& other) const {
    if (other.x == 0 || other.y == 0) throw std::invalid_argument("Division by zero in Vector2 division");
    return Vector2(x / other.x, y / other.y);
  }

  static Vector2 ComponentMin(Vector2 a, Vector2 b) {
    return {
      std::fminf(a.x, b.x),
      std::fminf(a.y, b.y)
    };
  }
  static Vector2 ComponentMax(Vector2 a, Vector2 b) {
    return {
      std::fmaxf(a.x, b.x),
      std::fmaxf(a.y, b.y)
    };
  }
  static Vector2 MagnitudeMin(Vector2 left, Vector2 right) {
    return left.lengthS() < right.lengthS() ? left : right;
  }
  static Vector2 MagnitudeMax(Vector2 left, Vector2 right) {
    return left.lengthS() >= right.lengthS() ? left : right;
  }

  inline Vector2 clamp(Vector2 min, Vector2 max) {
    return {
      std::fmaxf(min.x, std::fminf(max.x, x)),
      std::fmaxf(min.y, std::fminf(max.y, y))
    };
  }
  inline float distance(Vector2 target) {
    return sqrtf(distanceS(target));
  }
  inline float distanceS(Vector2 target) {
    return pow(target.x - x, 2) + pow(target.y - y, 2);
  }
  inline float dot(Vector2 other) {
    return x * other.x + y * other.y;
  }
  inline float perpDot(Vector2 other) {
    return x * other.y - y * other.x;
  }
  inline Vector2 lerp(Vector2 other, float blend) {
    return {
      blend * (other.x - x) + x,
      blend * (other.y - y) + y
    };
  };
  inline Vector2 baryCentric(Vector2 b, Vector2 c, float u, float v) {
    const Vector2 a = *this;
    return a + u * (b - a) + v * (c - a);
  }
};

#ifndef __GDF_VECTOR2_STATIC__
#define __GDF_VECTOR2_STATIC__

// shouldve worked bidirectionally imo
Vector2 operator*(float scalar, Vector2 lovely) {
  return lovely * scalar;
}

const Vector2 Vector2::UnitX = Vector2(1, 0);
const Vector2 Vector2::UnitY = Vector2(0, 1);
const Vector2 Vector2::Zero = Vector2(0, 0);
const Vector2 Vector2::One = Vector2(1, 1);
#endif