#pragma once

#include "../../graphics/containers/OsuOverlayContainer.hpp"
#include "../../../utils.hpp"
#include "SettingsSidebar.hpp"

class SettingsPanel : public OsuOverlayContainer {
public:
    const float CONTENT_MARGINS = 20;

    const float TRANSITION_LENGTH = 600;

    private: const float sidebar_width = SettingsSidebar::EXPANDED_WIDTH;

public: 
    /// <summary>
    /// The width of the settings panel content, excluding the sidebar.
    /// </summary>
    const float PANEL_WIDTH = 400;

    /// <summary>
    /// The full width of the settings panel, including the sidebar.
    /// </summary>
    const float WIDTH = sidebar_width + PANEL_WIDTH;

    bool init();
    static SettingsPanel* create() {
        create_class(SettingsPanel, init);
    }
    void onOpen() override;
    void onClose() override;
};