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

    addSettings(({
      auto s = SettingsSlider::create({}); 
      s->setContentWidth(100, frameworks::Unit::Percent);
      s;
    })/*VolumeSettings::create()*/);
    
    return true;
  }

  $defaultCreate(AudioSection);
};

GDL_NS_END