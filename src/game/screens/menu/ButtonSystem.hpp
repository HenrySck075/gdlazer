#pragma once
#include <Geode/Geode.hpp>
#include <vector>
#include "OsuLogo.hpp"
#include "MainMenuButton.hpp"
#include "ButtonConstants.hpp"
using namespace geode::prelude;


class ButtonSystem : public CCLayer
{
  public:
    float WEDGE_WIDTH = 20;

    //Action<ButtonSystemState>? StateChanged;

    ButtonCallback onEditBeatmap;
    ButtonCallback onEditSkin;
    ButtonCallback onExit;
    ButtonCallback onBeatmapListing;
    ButtonCallback onSolo;
    ButtonCallback onSettings;
    ButtonCallback onMultiplayer;
    ButtonCallback onPlaylists;
    ButtonCallback onDailyChallenge;

  private:
    OsuLogo* logo;

  public:
    /// <summary>
    /// Assign the <see cref="OsuLogo"/> that this ButtonSystem should manage the position of.
    /// </summary>
    /// <param name="logo">The instance of the logo to be assigned. If null, we are suspending from the screen that uses this ButtonSystem.</param>
    void setOsuLogo(OsuLogo* logo); 

    static ButtonSystem* create(OsuLogo* logo) {
        create_class(ButtonSystem, init, logo);
    }
    bool init(OsuLogo* logo);

  private:
    MenuLayer* m_menuLayerPtr;
    // those with leftAnchor being false will be placed before the logo
    MainMenuButton* postButtonSetup(MainMenuButton* btn);
    void resize(ReactiveNode* calledNode, Property props);

    /*priv read*/ MainMenuButton* backButton;

    /*priv read*/ std::vector<MainMenuButton*> buttonsTopLevel = {};
    /*priv read*/ std::vector<MainMenuButton*> buttonsPlay = {};
    /*priv read*/ std::vector<MainMenuButton*> buttonsEdit = {};

    const char* sampleBackToLogo = "back-to-logo.wav"_spr;
    const char* sampleLogoSwoosh = "osu-logo-swoosh.wav"_spr;

};


