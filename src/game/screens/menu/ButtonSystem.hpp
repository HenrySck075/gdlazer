#pragma once
#include <Geode/Geode.hpp>
#include <vector>
#include "OsuLogo.hpp"
#include "MainMenuButton.hpp"
#include "ButtonConstants.hpp"
#include "../../../helpers/CustomActions.hpp"
#include "../../../framework/graphics/containers/Container.hpp"
#include "ButtonArea.hpp"
using namespace geode::prelude;

class OsuGame;
#include "../../OsuGame.hpp"

class ButtonSystem : public Container
{
  public:
    float WEDGE_WIDTH = 20;

    //Action<ButtonSystemState>? StateChanged;

  private:
    OsuLogo* logo;

  public:
    ButtonArea* area;
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
    CreatorLayer* m_creatorLayerPtr;
    //void resize(ReactiveNode* calledNode, Property props);

    /*priv read*/ MainMenuButton* backButton;


    const char* sampleBackToLogo = "back-to-logo.wav"_spr;
    const char* sampleLogoSwoosh = "osu-logo-swoosh.wav"_spr;

};


