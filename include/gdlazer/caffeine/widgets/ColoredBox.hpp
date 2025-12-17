#pragma once

#include <memory>
#include <cstdint>
#include "../rendering/proxy_box/ProxyBox.hpp"
#include "framework/Widget.hpp"

namespace caffeine {

// ============================================================================
// Color type
// ============================================================================

struct Color {
  uint32_t value;  // ARGB format

  Color() : value(0xFF000000) {}  // Black with full opacity
  Color(uint32_t v) : value(v) {}
  
  static Color fromARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    return Color((static_cast<uint32_t>(a) << 24) |
                 (static_cast<uint32_t>(r) << 16) |
                 (static_cast<uint32_t>(g) << 8) |
                 static_cast<uint32_t>(b));
  }

  static Color fromRGB(uint8_t r, uint8_t g, uint8_t b) {
    return fromARGB(255, r, g, b);
  }

  uint8_t getAlpha() const { return (value >> 24) & 0xFF; }
  uint8_t getRed() const { return (value >> 16) & 0xFF; }
  uint8_t getGreen() const { return (value >> 8) & 0xFF; }
  uint8_t getBlue() const { return value & 0xFF; }

  bool operator==(const Color& other) const { return value == other.value; }
  bool operator!=(const Color& other) const { return !(*this == other); }
};

// ============================================================================
// RenderColoredBox
// ============================================================================

class RenderColoredBox : public RenderProxyBox {
protected:
  Color m_color;
  bool m_isAntiAlias;

public:
  RenderColoredBox(Color color = Color(), bool isAntiAlias = true, 
                   std::shared_ptr<RenderBox> child = nullptr)
    : RenderProxyBox(child), m_color(color), m_isAntiAlias(isAntiAlias) {}

  Color getColor() const { return m_color; }
  void setColor(Color color) {
    if (color != m_color) {
      m_color = color;
      markNeedsPaint();
    }
  }

  bool getIsAntiAlias() const { return m_isAntiAlias; }
  void setIsAntiAlias(bool isAntiAlias) {
    if (isAntiAlias != m_isAntiAlias) {
      m_isAntiAlias = isAntiAlias;
      markNeedsPaint();
    }
  }

  void paint(SkCanvas* canvas) override;

  virtual ~RenderColoredBox() = default;
};

// ============================================================================
// ColoredBox Widget
// ============================================================================

class ColoredBox : public SingleChildRenderObjectWidget {
protected:
  Color m_color;
  bool m_isAntiAlias;

public:
  ColoredBox(Color color, Widget* child = nullptr, bool isAntiAlias = true)
    : SingleChildRenderObjectWidget(child), m_color(color), m_isAntiAlias(isAntiAlias) {}

  Color getColor() const { return m_color; }
  bool getIsAntiAlias() const { return m_isAntiAlias; }

  std::shared_ptr<RenderObject> createRenderObject() override;
  void updateRenderObject(std::shared_ptr<BuildContext> context,
                          std::shared_ptr<RenderObject> renderObject) override;

  virtual ~ColoredBox() = default;
};

}  // namespace caffeine
