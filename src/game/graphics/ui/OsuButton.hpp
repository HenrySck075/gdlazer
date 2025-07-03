#pragma once

#include "../../../frameworks/graphics/containers/ClickableContainer.hpp"
#include "../../../frameworks/graphics/color/Color4.hpp"
#include "../OsuColor.hpp"

GDL_NS_START

class OsuButton : public frameworks::ClickableContainer {
private:
  struct ConstructorKwargs {
    Color4 backgroundColor = OsuColor::BlueDark;
  };

protected:
  geode::Ref<frameworks::Container> m_hoverContainer;
  geode::Ref<frameworks::Container> m_flashContainer;

public:
  static OsuButton* create(GDF_KWARGS) {
    $createClass2(OsuButton);
  }

  bool init(GDF_KWARGS);
};

GDL_NS_END