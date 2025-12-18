#include "gdlazer/caffeine/widgets/SizedBox.hpp"
#include "gdlazer/caffeine/rendering/proxy_box/sizer.hpp"
#include <memory>

namespace caffeine {


void SizedBox::updateRenderObject(RefNauseam<BuildContext>, RefNauseam<RenderObject> renderObject) {
  auto t = dynamic_cast<RenderConstrainedBox*>(renderObject.get());
  if (t) {
    t->setAdditionalConstraints(m_additionalConstraints);
  }
};


RefNauseam<RenderObject> SizedBox::createRenderObject() {
  return new RenderConstrainedBox(m_additionalConstraints);
}


}  // namespace caffeine

