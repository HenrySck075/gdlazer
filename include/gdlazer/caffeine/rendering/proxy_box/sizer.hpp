#pragma once

/// Every RenderProxyBox that affects the size
#include "gdlazer/caffeine/rendering/object/RenderObject.hpp"
#include "gdlazer/caffeine/rendering/proxy_box/ProxyBox.hpp"
namespace caffeine {
  class RenderConstrainedBox : public RenderProxyBox {
  public:
    RenderConstrainedBox(const BoxConstraints& additionalConstraints, std::shared_ptr<RenderBox> child = nullptr)
      : RenderProxyBox(child), m_additionalConstraints(additionalConstraints) {}
    BoxConstraints getAdditionalConstraints() const {
      return m_additionalConstraints;
    }
    void setAdditionalConstraints(BoxConstraints constraints) {
      if (constraints == m_additionalConstraints) return;
      m_additionalConstraints = constraints;
      markNeedsLayout();
    }

    void performLayout() override {
      if (m_child) {
        m_child->layout(m_additionalConstraints.enforce(m_constraints), true);
        m_size = m_child->getSize();
      } else {
        m_size = m_additionalConstraints.enforce(m_constraints).smallest();
      }
    }
  private:
    BoxConstraints m_additionalConstraints;
  };
}
