#pragma once
#include <fmt/base.h>
#define HOMOG2D_ENABLE_VRTP
#include "homog2d.hpp"
#include <fmt/core.h>

template<typename T>
struct fmt::formatter<h2d::Point2d_<T>> {
  constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }
  template <typename Context>
  constexpr auto format (h2d::Point2d_<T> const& point, Context& ctx) const {
      return format_to(ctx.out(), "({}, {})", point.getX(), point.getY());
  }
};

template<typename T>
struct fmt::formatter<h2d::FRect_<T>> {
  constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }
  template <typename Context>
  constexpr auto format (h2d::FRect_<T> const& rect, Context& ctx) const {
      return format_to(ctx.out(), "[{}, {}]", rect.getPts().first, rect.getPts().second);
  }
};

template<typename T>
struct fmt::formatter<h2d::Circle_<T>> {
  constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }
  template <typename Context>
  constexpr auto format (h2d::Circle_<T> const& circ, Context& ctx) const {
      return format_to(ctx.out(), "( {}, R={} )", circ.getCenter(), circ.radius());
  }
};

template<typename T>
struct fmt::formatter<h2d::CPolyline_<T>> {
  constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }
  template <typename Context>
  constexpr auto format (h2d::CPolyline_<T> const& poly, Context& ctx) const {
      return format_to(ctx.out(), "/(closed)Polyline: {}\\", poly.getPts());
  }
};

template<typename T>
struct fmt::formatter<h2d::CommonType_<T>> {
  constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }
  template <typename Context>
  constexpr auto format (h2d::CommonType_<T> const& variant, Context& ctx) const {
    #define shadyCode(type) if (std::holds_alternative<type<T>>(variant)) return format_to(ctx.out(), "{}", std::get<type<T>>(variant))

    shadyCode(h2d::FRect_);
    shadyCode(h2d::Point2d_);
    shadyCode(h2d::Circle_);
    return format_to(ctx.out(), "(unsupported h2d type)");
  }
};
