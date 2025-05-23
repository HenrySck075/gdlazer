#include "ButtonSystem.hpp"
#include "../../graphics/OsuIcon.hpp"
#include "../../overlays/dialog/PopupDialog.hpp"
#include "../../overlays/toolbar/ToolbarToggleButton.hpp"
#include "../PlaygroundScreen.hpp"
#include "ButtonConstants.hpp"
#include "../../OsuGame.hpp"

/*
class SongSelect;
#include "../select/SongSelect.hpp"
*/
bool tempForceReplace2 = false;
#include <Geode/modify/CCDirector.hpp>
struct area51 : Modify<area51, CCDirector>{
  bool replaceScene(CCScene* scene) {
    if (tempForceReplace2 && !geode::cast::typeinfo_cast<CCTransitionScene*>(getRunningScene())) {
      tempForceReplace2 = false;
      return CCDirector::replaceScene(CCTransitionFade::create(0.5,gdlazer::game::OsuGame::get()));
    }
    else return CCDirector::replaceScene(scene);
  }
};

#include <Geode/modify/LevelSelectLayer.hpp>
class $modify(LevelSelectLayer) {
  void onBack(CCObject* s) {
    tempForceReplace2 = true;
    LevelSelectLayer::onBack(s);
  }
};
GDL_NS_START
using namespace frameworks;
float ButtonSystem::s_wedgeWidth = 20;

void ButtonSystem::setOsuLogo(OsuLogo* logo) {
  this->m_logo = logo;
  /*
  if (logo != nullptr)
  {
    this->logo.Action = onOsuLogo;

    // osuLogo.SizeForFlow relies on loading to be complete.
    buttonArea.Flow.Position = ccp(s_wedgeWidth * 2 - (BUTTON_WIDTH + logo.SizeForFlow / 4), 0);

    updateLogoState();
  }
  else
  {
    // We should stop tracking as the facade is now out of scope.
    
    logoTrackingContainer.StopTracking();
  }
  */
}

bool ButtonSystem::init(OsuLogo* logo) {
  if (!Container::init()) return false;
  setOsuLogo(logo);
  m_menuLayerPtr = GameManager::sharedState()->m_menuLayer;
  m_creatorLayerPtr = CreatorLayer::create();

  //float w = CCDirector::sharedDirector()->getWinSize().width;
  CCPoint an = {-s_wedgeWidth*4,c_buttonAreaHeight/2};
  m_area = ButtonArea::create(an);
  m_area->setID("buttonarea");
  addChild(m_area);

  // because cocos2d-x does not allows a node to be in multiple parent (everyone knows that)
  #define backBtnCreate MainMenuButton::create( \
    "Back",                   \
    "back-to-top.wav"_spr,          \
    OsuIcon::PrevCircle,            \
    Color4(51, 58, 94, 255),          \
    [this](Container*j){m_area->pop();},       \
    {enumKeyCodes::KEY_Escape}        \
  )
  
  #define $cca(...) CCArray::create(__VA_ARGS__, nullptr)
  // daily/weekly levels
  m_area->constructButtons($cca(
    backBtnCreate,
    MainMenuButton::create(
      "Daily", 
      "button-default-select.wav"_spr, 
      OsuIcon::ModRelax, 
      Color4(102, 68, 204, 255),
      [this](Container*j){m_creatorLayerPtr->onDailyLevel(this);},
      {enumKeyCodes::KEY_D}
    ),
    MainMenuButton::create(
      "Weekly",
      "button-default-select.wav"_spr, 
      OsuIcon::ModSuddenDeath,
      Color4(94, 63, 186, 255), 
      [this](Container*j){m_creatorLayerPtr->onWeeklyLevel(this);},
      {enumKeyCodes::KEY_W}
    )
  ), "challenges");
  m_area->constructButtons($cca(
    backBtnCreate,
    MainMenuButton::create(
      "Main levels", 
      "button-default-select.wav"_spr, 
      OsuIcon::Player, 
      Color4(102, 68, 204, 255),
      [this](Container*j){
        //m_menuLayerPtr->onPlay(m_menuLayerPtr);
        if (Mod::get()->getSettingValue<bool>("devmode")) {
          //OsuGame::get()->pushScreen(SongSelect::create())->setZOrder(-7);
        } else {
          m_menuLayerPtr->onPlay(nullptr);
        }
      },
      {enumKeyCodes::KEY_M}
    ),
    MainMenuButton::create(
      "Saved levels", 
      "button-default-select.wav"_spr, 
      OsuIcon::Online, 
      Color4(94, 63, 186, 255), 
      [this](Container*j){m_creatorLayerPtr->onSavedLevels(this);},
      {enumKeyCodes::KEY_P}
    )
  ),"play_local");
  // play  
  m_area->constructButtons($cca(
    backBtnCreate,
    MainMenuButton::create(
      "Local", 
      "button-play-select.wav"_spr, 
      OsuIcon::Player, 
      Color4(102, 68, 204, 255),
      [this](Container*j){m_area->show("play_local");},
      {enumKeyCodes::KEY_P}
    ),
    MainMenuButton::create(
      "Multiplayer", 
      "button-default-select.wav"_spr, 
      OsuIcon::Online, 
      Color4(94, 63, 186, 255), 
      [this](Container*j){
        auto p = m_menuLayerPtr->getChildByID("bottom-menu")->getChildByID("dankmeme.globed2/main-menu-button");
        if (p) {
          typeinfo_cast<CCMenuItemSpriteExtra*>(p)->activate();
          return;
        }
        PopupDialog::createSimpleDialog(
          "Not implemented yet!", "mrow", 
          ":sphere_thumbsup:", "alr", 
          [](CCNode*wfhuio){CCApplication::sharedApplication()->openURL("https://www.pixiv.net/en/artworks/112989297");}
        )->show();
      },
      {enumKeyCodes::KEY_M}
    ),
    MainMenuButton::create(
      "Playlists", // this is lists
      "button-default-select.wav"_spr, 
      OsuIcon::Tournament, 
      Color4(94, 63, 186, 255), 
      [](CCNode*j){PopupDialog::createSimpleDialog(
        "hey guys robtopgames here", "geometry dash pdb in bio", 
        "yipee", "nuh uh", 
        [](CCNode*wfhuio){CCApplication::sharedApplication()->openURL("https://www.pixiv.net/en/artworks/120777141");}
      )->show();},
      {enumKeyCodes::KEY_L}
    ),
    MainMenuButton::create(
      "Challenges", // daily/weekly levels
      "button-play-select.wav"_spr, 
      OsuIcon::DailyChallenge,
      Color4(94, 63, 186, 255), 
      [this](Container*j) {m_area->show("challenges");},
      {enumKeyCodes::KEY_C}
    )
  ), "play");

  // edit
  m_area->constructButtons($cca(
    backBtnCreate,
    MainMenuButton::create(
      "New", 
      "button-default-select.wav"_spr, 
      OsuIcon::ChangelogA, 
      Color4(238, 170, 0, 255), 
      [](CCNode*fowehui) {
        LevelBrowserLayer::create(GJSearchObject::create(SearchType::MyLevels))->onNew(fowehui);        
      },
      {enumKeyCodes::KEY_B, enumKeyCodes::KEY_E}
    ),
    MainMenuButton::create(
      "Created levels", 
      "button-default-select.wav"_spr, 
      OsuIcon::Collections, 
      Color4(220, 160, 0, 255), 
      [this](Container*j){m_creatorLayerPtr->onMyLevels(this);},
      {enumKeyCodes::KEY_S}
    )
  ), "edit");
  
  // debug
  m_area->constructButtons($cca(
    backBtnCreate,
    MainMenuButton::create(
      "Open toolbar", 
      "button-default-select.wav"_spr, 
      OsuIcon::Beatmap, 
      Color4(165, 204, 0, 255), 
      [](CCNode* j) {
        OsuGame::get()->showToolbar();
        //->setUserObject("osugame.show-toolbar"_spr, CCBool::create(true));
      }
    ),
    MainMenuButton::create(
      "Close toolbar", 
      "button-default-select.wav"_spr, 
      OsuIcon::Beatmap, 
      Color4(154, 198, 0, 255), 
      [](CCNode* j) {
        OsuGame::get()->hideToolbar();
        //CCDirector::sharedDirector()->getRunningScene()->setUserObject("osugame.show-toolbar"_spr, CCBool::create(false));
      }
    ),
    MainMenuButton::create(
      "Playground", 
      "button-default-select.wav"_spr, 
      OsuIcon::Debug, 
      Color4(154, 198, 0, 255), 
      [](CCNode* j) {
        OsuGame::get()->pushScreen(PlaygroundScreen::create());
        //CCDirector::sharedDirector()->getRunningScene()->setUserObject("osugame.show-toolbar"_spr, CCBool::create(false));
      }
    )
  ), "debug");

  // toplevel
  m_area->constructButtons($cca(
    MainMenuButton::create(
      "Settings", 
      "osu-logo-downbeat.wav"_spr, 
      OsuIcon::Settings, 
      Color4(85, 85, 85, 255), 
      [this](Container*j){
        static_cast<ToolbarToggleButton*>(OsuGame::get()->getChildByIDRecursive("settings"))->select();
      }
    ),
    MainMenuButton::create(
      "Play", 
      "button-play-select.wav"_spr, 
      OsuIcon::Logo, 
      Color4(102, 68, 204, 255), 
      [this](Container* j) {m_area->show("play");},
      {enumKeyCodes::KEY_P, enumKeyCodes::KEY_M, enumKeyCodes::KEY_L}
    ),
    MainMenuButton::create(
      "Edit", 
      "button-play-select.wav"_spr, 
      OsuIcon::EditCircle, 
      Color4(238, 170, 0, 255), 
      [this](Container* j) {m_area->show("edit");},
      {enumKeyCodes::KEY_E}
    ),
    MainMenuButton::create(
      "Browse", 
      "button-play-select.wav"_spr, 
      OsuIcon::Beatmap, 
      Color4(165, 204, 0, 255), 
      [this](Container* j) {m_area->show("debug");},
      {enumKeyCodes::KEY_B, enumKeyCodes::KEY_D}
    ),
    MainMenuButton::create(
      "Exit", 
      "", 
      OsuIcon::CrossCircle, 
      Color4(238, 51, 153, 255), 
      [this](Container* j) {
        PopupDialog::createSimpleDialog(
          "Are you sure you want to exit GD?",
          "Last chance to turn back",

          "lemme out i need to take a bath",
          "nvm the 20-20-20 rule sucks", [this](Container* s) {m_menuLayerPtr->endGame(); }
        )->show();
      },
      {enumKeyCodes::KEY_Q  }
    )
  ), "toplevel");

  m_area->show("toplevel");
  
  setAnchor(Anchor::Center);
  setContentSize({100,c_buttonAreaHeight},frameworks::Unit::Percent,frameworks::Unit::OpenGL);
  logo->setZOrder(1);
  logo->setPosition({an.x,0});
  logo->setScale(0.4);
  logo->setAnchor(Anchor::Center);
  logo->addListener<frameworks::MouseEvent>([this](frameworks::MouseEvent* e){
    if (e->m_eventType != frameworks::MouseEventType::Click) return true;
    auto cur = m_area->getCurrent();
    if (cur.has_value()) 
      geode::cast::typeinfo_cast<MainMenuButton*>(
        getChildByIDRecursive("buttonarea_"+cur.value())->getChildByTag(2)->getChildren()->lastObject()
      )->click();
    return true;
  });
  
  //setPositionX(0);
  //auto me = CCMenu::createWithItem(logo);
  //me->setPosition({0,0});
  addChild(logo);
  setAnchorPoint({0.5,0.5});

  addListener<frameworks::NodeLayoutUpdated>([this](frameworks::NodeLayoutUpdated* e){
    geode::log::debug("[ButtonSystem]: {}", getChildren());
    return true;
  });

  return true;
}
GDL_NS_END
/*
void ButtonSystem::resize(ReactiveNode* idc, Property prop) {
  if (prop == Property::Size) idc->getParent()->updateLayout();
}
*/
