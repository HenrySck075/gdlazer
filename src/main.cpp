#include "game/screens/menu/MainMenu.hpp"
#include "game/screens/menu/intro/IntroTriangles.hpp"
#include <Geode/Geode.hpp>
#include <random>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>

bool g_screenPushed = false;
struct e : public Modify<e, MenuLayer> {
  bool init() {
    if (!MenuLayer::init()) return false;

    auto myButton = CCMenuItemSpriteExtra::create(
      CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
      this,
      menu_selector(e::onMyButton)
    );
    auto menu = this->getChildByID("bottom-menu");
    menu->addChild(myButton);

    myButton->setID("my-button"_spr);

    menu->updateLayout();

    auto d = CCDirector::get();

    log::debug("{} {}", d->getWinSize(), cocos2d::CCDirector::get()->getOpenGLView()->getFrameSize());

    return true;
  };
  void onMyButton(CCObject*) {
    auto g = gdlazer::game::OsuGame::get();
    cocos2d::CCDirector::get()->pushScene(g);
    if (!g_screenPushed) g->pushScreen(GDL_NS::IntroTriangles::create());
    g_screenPushed = true;
  }
};

void showCursor(bool state) {
#ifdef GEODE_IS_WINDOWS
  cocos2d::CCDirector::get()->getOpenGLView()->showCursor(false);
#endif
}

bool s_replaceSceneDisabled = false;

#include <Geode/modify/CCDirector.hpp>

class $modify(cocos2d::CCDirector) {
  static void onModify(auto& self) {
    if (!self.setHookPriorityPre("cocos2d::CCDirector::replaceScene", Priority::VeryEarlyPre)) {
      geode::log::warn("stuypd"); 
    };
  }
  bool replaceScene(cocos2d::CCScene* scene) {
    if (!s_replaceSceneDisabled) return cocos2d::CCDirector::replaceScene(scene);
    return true;
  }
};
#include <Geode/modify/GameManager.hpp>
class $modify(GameManager) {
  void fadeInMenuMusic() {}
};
#include <Geode/modify/LevelBrowserLayer.hpp>
class $modify(LevelBrowserLayer) {
  void onBack(cocos2d::CCObject*) {
    cocos2d::CCDirector::get()->replaceScene(gdlazer::game::OsuGame::get());
  }
};

#include <Geode/modify/LoadingLayer.hpp>
class $modify(LoadingLayer) {
  static void onModify(auto& self) {
    if (!self.setHookPriorityPre("LoadingLayer::loadAssets", Priority::VeryEarlyPre)) {
      geode::log::warn("Failed to set hook priority for LoadingLayer::loadAssets.");
    }
    if (!self.setHookPriorityAfterPost("LoadingLayer::init", Loader::get()->getLoadedMod("geode.loader"))) {
      geode::log::warn("Failed to set hook priority for LoadingLayer::init.");
    }
  }
  void loadAssets() {
    bool lastStep = m_loadStep == 0xe;
    if (lastStep) {
      s_replaceSceneDisabled = true;
    }
    LoadingLayer::loadAssets();
    if (lastStep) {
      s_replaceSceneDisabled = false;
      showCursor(false);

      auto g = gdlazer::game::OsuGame::get();
      cocos2d::CCDirector::get()->pushScene(g);
      if (!g_screenPushed) g->pushScreen(GDL_NS::IntroTriangles::create());
      g_screenPushed = true;
    }
  }
  bool init(bool p0) {
    if (!LoadingLayer::init(p0)) return false;

    auto yoffset = CCDirector::get()->getWinSize().height * 2;
    auto xpos = CCDirector::get()->getWinSize().width - 10;

    auto smallLabel = static_cast<cocos2d::CCLabelBMFont*>(getChildByID("geode-small-label"));
    auto smallerLabel = static_cast<cocos2d::CCLabelBMFont*>(getChildByID("geode-small-label-2"));

    setVisible(false);

    queueInMainThread([this, smallLabel, smallerLabel, yoffset, xpos]{
      smallLabel->setPosition({xpos, 10 + yoffset});
      smallLabel->setAnchorPoint({1,0});
      smallLabel->setFntFile("Torus-Regular.fnt"_spr);
      smallLabel->setScale(.3f);
      
      smallerLabel->setPosition({xpos, 25 + yoffset});
      smallerLabel->setFntFile("Torus-Regular.fnt"_spr);
      smallerLabel->setAnchorPoint({1,0});
      smallerLabel->setScale(.2f);
      
      setPositionY(-yoffset);
      setVisible(true);
    });
    return true;
  }
};


/// The "nothing useful feature": Replace the mod dev name with any of these hardcoded names.

static const char* c_smug[] = {
  "Honoka Kosaka",
  "Eli Ayase",
  "Kotori Minami",
  "Nico Yazawa",
  "Maki Nishikino",
  "Hanayo Koizumi",
  "Umi Sonoda",
  "Nozomi Tojo",
  "Rin Hoshizora",
  "Tsubasa Kira",
  "Anju Yuki",
  "Erena Todo",
  "Chika Takami",
  "Riko Sakurauchi",
  "Dia Kurosawa",
  "Yoshiko Tsushima",
  "You Watanabe",
  "Kanan Matsuura",
  "Hanamaru Kunikida",
  "Ruby Kurosawa",
  "Mari Ohara",
  "Sarah Kazuno",
  "Leah Kazuno",
  "Yu Takasaki",
  "Ayumu Uehara",
  "Setsuna Yuki",
  "Ai Miyashita",
  "Kasumi Nakasu",
  "Shizuku Osaka",
  "Rina Tennoji",
  "Emma Verde",
  "Karin Asaka",
  "Kanata Konoe",
  "Kanon Shibuya",
  "Keke Tang",
  "Chisato Arashi",
  "Sumire Heanna",
  "Ren Hazuki",
  "Shioriko Mifune",
  "Mia Taylor",
  "Lanzhu Zhong",
  "Yuna Hijirisawa",
  "Mao Hiiragi",
  "Hana Hazuki",
  "Kaoruko Mifune",
  "Kinako Sakurakoji",
  "Mei Yoneme",
  "Shiki Wakana",
  "Natsumi Onitsuka",
  "Wien Margarete",
  "Kaho Hinoshita",
  "Kozue Otomune",
  "Sayaka Murano",
  "Tsuzuri Yugiri",
  "Rurino Osawa",
  "Megumi Fujishima",
  "Tomari Onitsuka",
  "Sachi Ogami",
  "Ginko Momose",
  "Kosuzu Kachimachi",
  "Hime Anyoji",
  "Rurika Tsubaki",
  "Anzu Takizawa",
  "Yuzuha Sumeragi",
  "Misuzu Wakatsuki",
  "Yukino Hojo",
  "Toa Kurusu",
  "Hikaru Amakusa",
  "Rena Suzuka",
  "Maya Mikasa",
  "Sayaka Harukaze",
  "Izumi Katsuragi",
  "Ceras Yanagida Lilienfeld",
  "Polka Takahashi",
  "Mai Azabu",
  "Akira Goto",
  "Hanabi Komagata",
  "Miracle Kanazawa",
  "Noriko Chofu",
  "Yukuri Harumiya",
  "Aurora Konohana",
  "Midori Yamada",
  "Shion Sasaki",

  "Henry Spheria", /// congrats
  "SoggyGunner"    /// hello geode
};

$on_mod(Loaded) {
  if (!geode::Mod::get()->getSettingValue<bool>("dont")) return;
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1,sizeof(c_smug)/sizeof(c_smug[0])); 
  int i = dist(rng)-1;
  auto met = geode::Mod::get()->getMetadata();
  met.setDevelopers({c_smug[i]});
  geode::Mod::get()->setMetadata(met);
}
