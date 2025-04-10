#include "ToolbarToggleButton.hpp"
#include "../../graphics/OsuColor.hpp"
#include "../../../framework/graphics/containers/OverlayContainer.hpp"

bool ToolbarToggleButton::init(IconConstructor icon, std::string text, std::string sub, AxisAlignment align, std::string overlayName) {
  ToolbarButton::init(icon, text, sub, align);
  // since ToolbarToggleButton is actually ToolbarOverlayToggleButton, why not?
  addListener("overlayEvent", [this,overlayName](NodeEvent* e){
    auto ev = static_cast<OverlayEvent*>(e);
    if (ev->getEventType() == OverlayEvent::Type::Popin && ev->getOverlay()->getName() == overlayName) {
      ToolbarToggleButton::select();
    } else {
      ToolbarToggleButton::deselect();
    }
  });
  toggleBg = Container::create();
  toggleBg->setColor(OsuColor::Carmine.opacity(180));
  toggleBg->setContentSizeWithUnit(CCSize(100,100), Unit::Percent, Unit::Percent);
  toggleBg->setOpacity(0);
  bgWrapper->addChild(toggleBg,-4);
  return true;
}

void ToolbarToggleButton::onClick(MouseEvent* event) {
  ToolbarButton::onClick(event);
  if (toggled) {
    deselect();
  }
  else {
    select();
  }
}
