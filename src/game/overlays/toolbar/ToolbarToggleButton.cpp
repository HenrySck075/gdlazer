#include "ToolbarToggleButton.hpp"
#include "../../graphics/OsuColor.hpp"
#include "../../../frameworks/graphics/containers/OverlayContainer.hpp"

GDL_NS_START
using namespace frameworks;

bool ToolbarToggleButton::init(IconConstructor icon, std::string text, std::string sub, AxisAlignment align, std::string overlayName) {
  ToolbarButton::init(icon, text, sub, align);
  // since ToolbarToggleButton is actually ToolbarOverlayToggleButton, why not?
  addListener<OverlayEvent>([this,overlayName](OverlayEvent* ev){
    if (ev->getEventType() == OverlayEvent::Type::Popin && ev->getOverlay()->getName() == overlayName) {
      ToolbarToggleButton::select();
    } else {
      ToolbarToggleButton::deselect();
    }
    return true;
  });
  addListener<MouseEvent>([this](MouseEvent* e){
    if (e->m_eventType == MouseEventType::Click) {
      if (toggled) {
        deselect();
      }
      else {
        select();
      }
    }
    return true;
  });
  toggleBg = Container::create();
  toggleBg->setBackgroundColor(OsuColor::Carmine.opacity(180));
  toggleBg->setContentSize({92,92}, Unit::Percent);
  toggleBg->setAnchor(Anchor::Center);
  toggleBg->setAnchorPoint({0.5,0.5});
  toggleBg->setOpacity(0);
  toggleBg->setBorderRadius(4);
  toggleBg->setClippingEnabled(true);

  addChild(toggleBg,-4);
  return true;
}
GDL_NS_END
