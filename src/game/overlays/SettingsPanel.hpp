#pragma once

#include "../graphics/containers/OsuOverlayContainer.hpp"
#include "../graphics/containers/SectionsContainer.hpp"
#include "../../utils.hpp"
#include "settings/SettingsSidebar.hpp"

class SettingsPanel : public OsuOverlayContainer {
protected:
    SettingsSidebar* sidebar;
public:
    const float CONTENT_MARGINS = 20;

    static const float TRANSITION_LENGTH;

    private: static const float sidebar_width;

public: 
    /// <summary>
    /// The width of the settings panel content, excluding the sidebar.
    /// </summary>
    static const float PANEL_WIDTH;

    /// <summary>
    /// The full width of the settings panel, including the sidebar.
    /// </summary>
    static const float WIDTH;

    bool init();
    static SettingsPanel* create() {
        create_class(SettingsPanel, init);
    }
    virtual CCArrayExt<Container*> createSections() {return CCArray::create();};
    void onOpen() override;
    void onClose() override;
    void onDismiss() override;
};

// SectionsContainer with the fade stuff idk
class SettingsSections : public SectionsContainer {
public:
    void onSectionSelect(Container* old, Container* new_) override;
    default_create(SettingsSections);
};
