#include "gdlazer/caffeine/widgets/SizedBox.hpp"
#include "gdlazer/caffeine/rendering/proxy_box/sizer.hpp"
#include <memory>

namespace caffeine {


void SizedBox::updateRenderObject(std::shared_ptr<BuildContext>, std::shared_ptr<RenderObject> renderObject) {
  auto t = std::dynamic_pointer_cast<RenderConstrainedBox>(renderObject);
  if (t) {
    t->setAdditionalConstraints(m_additionalConstraints);
  }
};


std::shared_ptr<RenderObject> SizedBox::createRenderObject() {
  return std::make_shared<RenderConstrainedBox>(m_additionalConstraints);
}


}  // namespace caffeine

