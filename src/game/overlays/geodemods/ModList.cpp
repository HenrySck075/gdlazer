#include "ModListContainer.hpp"


GDL_NS_START


ModEntry modEntryFromModItemNode(CCNode* node) {
  auto infoContainer = node->getChildByID("info-container");
  auto viewMenu = node->getChildByID("view-menu");
  return {
    .icon = static_cast<geode::LazySprite*>(getChildByIdTree(node, {"logo-sprite", 0})),
    .name = static_cast<CCLabelBMFont*>(getChildByIdTree(infoContainer, {"title-container", "title-label"}))->getString(),
    .version = static_cast<CCLabelBMFont*>(getChildByIdTree(infoContainer, {"title-container", "version-label"}))->getString(),
    .developers = static_cast<CCLabelBMFont*>(getChildByIdTree(infoContainer, {"developers-menu", "developers-button", "developers-label"}))->getString(),

    .viewButton = static_cast<CCMenuItemSpriteExtra*>(viewMenu->getChildByID("view-button")),

    .restartRequired = infoContainer->getChildByID("restart-required-label")->isVisible(),
    .enabled = ({
      auto toggler = static_cast<CCMenuItemToggler*>(viewMenu->getChildByID("enable-toggler"));

      !toggler || toggler->isToggled();
    }),
    .updateAvailable = viewMenu->getChildByID("update-button")->isVisible()
  };
}

ModList::ModListTask const& InstalledModList::loadNextModsList() {
  if (!m_modsLayer) {
    geode::log::error("Mods layer pointer is null, returning existing data instead.");
    return ModListTask::immediate(m_mods);
  }
  

  
  return ModListTask::immediate({});
}

InstalledModList* InstalledModList::get(CCLayer* modsLayer) {
  static auto ret = new InstalledModList(modsLayer);
  return ret;
}

GDL_NS_END