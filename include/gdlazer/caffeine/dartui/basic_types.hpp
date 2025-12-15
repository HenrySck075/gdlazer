#pragma once

// dart:ui

namespace caffeine {
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
    const float left, right, top, bottom;

    // several other helper variables (all const because ltrb are const)
    const float width, height, longestSide, shortestSide;
    const Offset 
      topLeft, topCenter, topRight, 
      centerLeft, center, centerRight,
      bottomLeft, bottomCenter, bottomRight;

    bool isFinite, isInfinite, isEmpty;

    Size size;

    Rect(float left, float top, float right, float bottom)
      : left(left), right(right), top(top), bottom(bottom),
        width(right - left), height(bottom - top),
        longestSide(width > height ? width : height),
        shortestSide(width < height ? width : height),
        // offsets
        topLeft({left, top}), topCenter({(left + right) / 2.0f, top}), topRight({right, top}),
        centerLeft({left, (top + bottom) / 2.0f}), center({(left + right) / 2.0f, (top + bottom) / 2.0f}), centerRight({right, (top + bottom) / 2.0f}),
        bottomLeft({left, bottom}), bottomCenter({(left + right) / 2.0f, bottom}), bottomRight({right, bottom}),
        // states
        isInfinite(left == -INFINITY || right == INFINITY || top == -INFINITY || bottom == INFINITY),
        isEmpty(width <= 0.0f || height <= 0.0f),

        size(width, height)
    {
      isFinite = !isInfinite;
    }

    static Rect fromLTWH(float left, float top, float width, float height) {
      return Rect(left, top, left+width, top+height);
    }
    static Rect fromCenter(Offset center, float width, float height) {
      return Rect(center.dx - width/2.0f, center.dy - height/2.0f,
                  center.dx + width/2.0f, center.dy + height/2.0f);
    }
    static Rect fromCircle(Offset center, float radius) {
      return Rect(center.dx - radius, center.dy - radius,
                  center.dx + radius, center.dy + radius);
    }
    // Construct the smallest rectangle that encloses the given offsets, treating them as vectors from the origin (top-left).
    static Rect fromPoints(const Offset& a, const Offset& b) {
      float left = a.dx < b.dx ? a.dx : b.dx;
      float right = a.dx > b.dx ? a.dx : b.dx;
      float top = a.dy < b.dy ? a.dy : b.dy;
      float bottom = a.dy > b.dy ? a.dy : b.dy;
      return Rect(left, top, right, bottom);
    }
  };

}





// Models formatting (with fmt)

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
    return fmt::format_to(ctx.out(), "Rect[ ({}, {}), ({}, {}) ]",
      r.left, r.top, r.right, r.bottom);
  }
};
