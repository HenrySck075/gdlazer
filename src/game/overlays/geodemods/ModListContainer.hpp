#pragma once

#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../overlays/OverlayColorProvider.hpp"
#include "Geode/ui/GeodeUI.hpp"
#include "ModItem.hpp"

/// Since the Geode Mods layer API is private for obvious reason

GDL_NS_START
class ModList {
protected:
  ModList(CCLayer* modsLayer) : m_modsLayer(modsLayer) {};
  ModList() : m_modsLayer(nullptr) {};
  CCLayer* m_modsLayer; // This is the geode mods layer
  std::vector<ModEntry> m_mods;
  
public:
  using ModListTask = geode::Task<std::vector<ModEntry>>;

  
  virtual ModListTask const& loadNextModsList() {};
};

class InstalledModList : public ModList { 
protected:
  InstalledModList(CCLayer* modsLayer) : ModList(modsLayer) {};
public:
  static InstalledModList* get(CCLayer* modsLayer);
  ModListTask const& loadNextModsList() override;
};

class ModListContainer : public frameworks::Container {
public:
  bool init(OverlayColorProvider* provider, ModList* list);
  static ModListContainer* create(OverlayColorProvider* provider, ModList* list) {
    $createClass(ModListContainer, init, provider, list);
  }
private:
  ModList* m_list;
};
GDL_NS_END