#pragma once

#include "../SettingsSection.hpp"
#include "../elements/SettingsSlider.hpp"
#include "../../../graphics/ui/OsuIcon.hpp"
GDL_NS_START
class VolumeSettings : public SettingsSubsection {
public:
  bool init() {
    if (!SettingsSubsection::init("Volume")) return false;

    // Add volume settings here
    addSettings(SettingsSlider::create({
      
    }));

    return true;
  }

  $defaultCreate(VolumeSettings);
};

class AudioSection : public SettingsSection {
public:
  bool init() {
    if (!SettingsSection::init("Audio", OsuIcon::Audio)) return false;

    addSettings(SettingsSlider::create({})/*VolumeSettings::create()*/);
    
    return true;
  }

  $defaultCreate(AudioSection);
};

GDL_NS_END