#pragma once
#include "../../framework/screens/Screen.hpp"
#include "../overlays/settings/SettingsButton.hpp"
#include "../graphics/ui/LoadingSpinner.hpp"
#include "../graphics/OsuColor.hpp"
#include "../../utils.hpp"
#include "../../framework/graphics/sprites/FontAwesome.hpp"

class Playground final : public Screen {
public:
  bool init();
  $default_create(Playground);
};
