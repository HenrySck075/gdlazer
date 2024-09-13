#pragma once
#include <Geode/utils/general.hpp>

namespace ToolbarConstants {
    float const HEIGHT = 
    #ifdef GEODE_IS_ANDROID
      80
    #else
      40
    #endif
    ;
    float const TOOLTIP_HEIGHT = 30;
}
