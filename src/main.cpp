#include "game/OsuGame.hpp"
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
#ifdef GEODE_IS_DESKTOP
  cocos2d::CCDirector::get()->getOpenGLView()->showCursor(false);
#endif
}

bool s_replaceSceneDisabled = false;


geode::Hook* g_showCursorHook = nullptr;
#ifdef GEODE_IS_DESKTOP
#include <Geode/modify/CCEGLView.hpp>
struct showCursorHook : public geode::Modify<showCursorHook, CCEGLView> {
  static void onModify(auto& self) {
    auto hook = self.getHook("cocos2d::CCEGLView::showCursor");
    
    g_showCursorHook = hook.unwrapOr(nullptr);
    if (g_showCursorHook) g_showCursorHook->setAutoEnable(false);
  }
  void showCursor(bool state) {
    if (auto instance = gdlazer::game::OsuGame::get(false)) {
      instance->setMouseVisibility(state);
    }
  }
};
#endif

#include <Geode/modify/CCDirector.hpp>
class $modify(cocos2d::CCDirector) {
  static void onModify(auto& self) {
    if (!self.setHookPriorityPre("cocos2d::CCDirector::replaceScene", Priority::VeryEarlyPre)) {
      geode::log::warn("stuypd"); 
    };
  }
  bool pushScene(cocos2d::CCScene* scene) {
    bool isgdlScene = scene == gdlazer::game::OsuGame::get(false);
    if (g_showCursorHook) {
      if (isgdlScene) getOpenGLView()->showCursor(false);
      if ((
        (isgdlScene) 
        ? g_showCursorHook->enable() 
        : g_showCursorHook->disable()
      ).isErr()) {
        geode::log::error("Cannot change state of cocos2d::CCEGLView::showCursor hook.");
      } else {
        if (!isgdlScene) getOpenGLView()->showCursor(true);
      }
    }
    return cocos2d::CCDirector::pushScene(scene);
  }
  bool replaceScene(cocos2d::CCScene* scene) {
    if (!s_replaceSceneDisabled) {
      bool isgdlScene = scene == gdlazer::game::OsuGame::get(false);
      if (g_showCursorHook) {
        if (isgdlScene) getOpenGLView()->showCursor(false);
        if ((
          (isgdlScene) 
          ? g_showCursorHook->enable() 
          : g_showCursorHook->disable()
        ).isErr()) {
          geode::log::error("Cannot change state of cocos2d::CCEGLView::showCursor hook.");
        } else {
          if (!isgdlScene) getOpenGLView()->showCursor(true);
        }
      }
      return cocos2d::CCDirector::replaceScene(scene);
    }
    return true;
  }
};

#include <Geode/modify/GameManager.hpp>
class $modify(GameManager) {
  void fadeInMenuMusic() {
    if (!s_replaceSceneDisabled) {
    }
  }
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
    if (!self.setHookPriorityPre("LoadingLayer::loadAssets", Priority::VeryLatePre)) {
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

      auto g = gdlazer::game::OsuGame::get();
      cocos2d::CCDirector::get()->pushScene(g);
      if (!g_screenPushed) g->pushScreen(GDL_NS::IntroTriangles::create());
      g_screenPushed = true;
      if (g_showCursorHook && g_showCursorHook->enable().isErr()) {
        geode::log::error("Cannot enable cocos2d::CCEGLView::showCursor hook");
      };
    }
  }
  bool init(bool p0) {
    if (!LoadingLayer::init(p0)) return false;
    cocos2d::CCDirector::get()->getOpenGLView()->showCursor(false);

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


/// The "nothing useful feature": Replace the mod dev name (chance to hit the actual name is low so good luck).

static const char* c_smug[] = {
  /// Love Live! School idol project 
  "Honoka Kosaka",
  "Kotori Minami",
  "Umi Sonoda",
  "Rin Hoshizora",
  "Hanayo Koizumi",
  "Maki Nishikino",
  "Nico Yazawa",
  "Eli Ayase",
  "Nozomi Tojo",
  "Tsubasa Kira",
  "Anju Yuki",
  "Erena Todo",
  "Chika Takami",
  "You Watanabe",
  "Riko Sakurauchi",
  "Ruby Kurosawa",
  "Hanamaru Kunikida",
  "Yoshiko Tsushima",
  "Mari Ohara",
  "Dia Kurosawa",
  "Kanan Matsuura",
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
  "Shioriko Mifune",
  "Mia Taylor",
  "Lanzhu Zhong",
  "Yuna Hijirisawa",
  "Mao Hiiragi",
  "Hana Hazuki",
  "Kaoruko Mifune",
  "Kanon Shibuya",
  "Keke Tang",
  "Chisato Arashi",
  "Sumire Heanna",
  "Ren Hazuki",
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
  "Ceras Yanagida Lilienfeld",
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

  /// BanG Dream!
  "Toyama Kasumi",
  "Hanazono Tae",
  "Ushigome Rimi",
  "Yamabuki Saaya",
  "Ichigaya Arisa",
  "Mitake Ran",
  "Aoba Moca",
  "Uehara Himari",
  "Udagawa Tomoe",
  "Hazawa Tsugumi",
  "Maruyama Aya",
  "Hikawa Hina",
  "Shirasagi Chisato",
  "Yamato Maya",
  "Wakamiya Eve",
  "Minato Yukina",
  "Hikawa Sayo",
  "Imai Lisa",
  "Udagawa Ako",
  "Shirokane Rinko",
  "Tsurumaki Kokoro",
  "Seta Kaoru",
  "Kitazawa Hagumi",
  "Matsubara Kanon",
  "Okusawa Misaki",
  "Michelle",
  "Kurata Mashiro",
  "Kirigaya Touko",
  "Hiromachi Nanami",
  "Futaba Tsukushi",
  "Yashio Rui",
  "Wakana Rei",
  "Satou Masuki",
  "Satou Masuki",
  "Nyubara Reona",
  "Tamade Chiyu",
  "Takamatsu Tomori",
  "Chihaya Anon",
  "Kaname Raana",
  "Nagasaki Soyo",
  "Shiina Taki",
  "Doloris",
  "Mortis",
  "Timoris",
  "Amoris",
  "Oblivionis",
  "Nakamachi Arale",
  "Miyanaga Nonoka",
  "Minetsuki Ritsu",
  "Sengoku Yuno",
  "Fuji Miyako",
  "Ushigome Yuri",
  "Uzawa Rii",
  "Wanibe Nanana",
  "Nijikki Hinako",
  "Umino Natsuki",
  "Taiko Satomi",
  "Kawabata Mayu",
  "Mori Fumika",
  "Sumita Mana",
  "Serizawa Mio",
  "Yamase Serina",
  "Koganei Shino",

  /// The IDOLM@STER
  "Aranzena Bassilbera",
  "Anastasia",
  "Arisa Mochida",
  "Chika Yokoyama",
  "Chinatsu Aikawa",
  "Clarice",
  "Erika Akanishi",
  "Fuka Asano",
  "Hasumi Nagatomi",
  "Hijiri Mochizuki",
  "Haruka Amami",
  "Mano Sakuragi",
  "Mai Fukuyama",
  "Makoto Kikuchi",
  "Kana Imai",
  "Chihaya Kisaragi",
  "Yayoi Takatsuki",
  "Miki Hoshii",
  "Yukiho Hagiwara",
  "Takane Shijou",
  "Nana Abe",
  "Azusa Miura",
  "Hibiki Ganaha",
  "Uzuki Shimamura",
  "Hiori Kazano",
  "Yukimi Sajo",
  "Mirai Kasuga",
  "Shizuka Mogami",
  "Tsubasa Ibuki",
  "Yuka Nakano",
  "Yukari Mizumoto",
  "Aiko Takamori",
  "Minami Nitta",
  "Kanako Mimura",
  "Riamu Yumemi",
  "Meguru Hachimiya",
  "Rin Shibuya",
  "Airi Totoki",
  "Mika Jougasaki",
  "Noriko Shiina",
  "Saori Okuyama",
  "Misato Manaka",
  "Miyuki Yanase",
  "Tsubaki Egami",
  "Nagisa Aino",
  "Yumi Aiba",
  "Miria Akagi",
  "Rika Jougasaki",
  "Nanami Asari",
  "Izumi Ohishi",
  "Toko Hattori",
  "Yukino Aihara",
  "Yusuke Aoi",
  "Kyosuke Aoi",
  "Suzaku Akai",

  "Henry Spheria" /// congrats
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
