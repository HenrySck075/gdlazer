#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include <memory>
#include "../rendering/proxy_box/ProxyBox.hpp"
#include "framework/Widget.hpp"

namespace caffeine {

// ============================================================================
// RenderOpacity
// ============================================================================

class RenderOpacity : public RenderProxyBox {
protected:
  double m_opacity;
  bool m_alwaysIncludeSemantics;

public:
  RenderOpacity(double opacity = 1.0, bool alwaysIncludeSemantics = false,
                RefNauseam<RenderBox> child = nullptr)
    : RenderProxyBox(child), m_opacity(opacity), m_alwaysIncludeSemantics(alwaysIncludeSemantics) {
    // Clamp opacity to [0, 1]
    if (m_opacity < 0.0) m_opacity = 0.0;
    if (m_opacity > 1.0) m_opacity = 1.0;
  }

  double getOpacity() const { return m_opacity; }
  void setOpacity(double opacity) {
    // Clamp to [0, 1]
    if (opacity < 0.0) opacity = 0.0;
    if (opacity > 1.0) opacity = 1.0;

    if (opacity == m_opacity) {
      return;
    }
    m_opacity = opacity;
    markNeedsPaint();
  }

  bool getAlwaysIncludeSemantics() const { return m_alwaysIncludeSemantics; }
  void setAlwaysIncludeSemantics(bool value) {
    if (value == m_alwaysIncludeSemantics) {
      return;
    }
    m_alwaysIncludeSemantics = value;
  }

  void paint(PaintingContext* context, const Offset& offset) override;

  virtual ~RenderOpacity() = default;
};

// ============================================================================
// Opacity Widget
// ============================================================================

class Opacity : public SingleChildRenderObjectWidget {
protected:
  double m_opacity;
  bool m_alwaysIncludeSemantics;

public:
  Opacity(double opacity, Widget* child = nullptr, bool alwaysIncludeSemantics = false)
    : SingleChildRenderObjectWidget(child), 
      m_opacity(opacity), 
      m_alwaysIncludeSemantics(alwaysIncludeSemantics) {
    // Clamp opacity to [0, 1]
    if (m_opacity < 0.0) m_opacity = 0.0;
    if (m_opacity > 1.0) m_opacity = 1.0;
  }

  double getOpacity() const { return m_opacity; }
  bool getAlwaysIncludeSemantics() const { return m_alwaysIncludeSemantics; }

  RefNauseam<RenderObject> createRenderObject() override;
  void updateRenderObject(RefNauseam<BuildContext> context,
                          RefNauseam<RenderObject> renderObject) override;

  virtual ~Opacity() = default;
};

}  // namespace caffeine
