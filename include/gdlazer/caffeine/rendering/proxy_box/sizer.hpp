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
  private:
    BoxConstraints m_additionalConstraints;
  };
}
