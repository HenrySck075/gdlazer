#pragma once
#include <Geode/Geode.hpp>
#include <vector>
#include "ButtonArea.hpp"
#include "OsuLogo.hpp"
#include "MainMenuButton.hpp"
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

    static ButtonSystem* create() {
        create_class(ButtonSystem, init);
    }
    bool init();

  private:
      // those with leftAnchor being false will be placed before the logo
      MainMenuButton* postButtonSetup(MainMenuButton* btn, bool leftAnchor = true);
      void resize(ReactiveNode* calledNode);

    ButtonArea* buttonArea;

    /*priv read*/ MainMenuButton* backButton;

    /*priv read*/ CCArrayExt<MainMenuButton*> buttonsTopLevel = {};
    /*priv read*/ CCArrayExt<MainMenuButton*> buttonsPlay = {};
    /*priv read*/ CCArrayExt<MainMenuButton*> buttonsEdit = {};

    const char* sampleBackToLogo = "back-to-logo.wav"_spr;
    const char* sampleLogoSwoosh = "osu-logo-swoosh.wav"_spr;



};


