#pragma once

#include "../../graphics/containers/OsuClickableContainer.hpp"
#include "../../graphics/OsuIcon.hpp"

class ToolbarButton : public OsuClickableContainer {
public:
    bool init(IconConstructor icon);
    void onMouseEnter() override;
    static ToolbarButton* create(IconConstructor icon) {}
};
