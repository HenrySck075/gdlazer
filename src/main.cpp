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
  "Shion Sasaki"
};

$on_mod(Loaded) {
  if (!geode::Mod::get()->getSettingValue<bool>("dont")) return;
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1,sizeof(c_smug)/sizeof(c_smug[0])); 
  int i = dist(rng);
  auto met = geode::Mod::get()->getMetadata();
  met.setDevelopers({c_smug[i]});
  geode::Mod::get()->setMetadata(met);
}