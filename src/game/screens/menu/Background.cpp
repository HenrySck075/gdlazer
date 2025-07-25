#include "Background.hpp"
#include "Geode/utils/web.hpp"
#include <chrono>
#include <matjson.hpp>
#include <random>

GDL_NS_START
using namespace frameworks;

bool Background::init() {
  Container::init();
  m_backgroundGetListener.bind([this](meow::Event* e){
    bool willSwitch = (m_background != nullptr);
    CCResizableSprite* oldBg = m_background;
    m_background = CCResizableSprite::create();
    m_backgroundName = *e->getValue();
    addChild(m_background);
    if (auto img = e->getValue()) {
      if (willSwitch) m_background->setOpacity(0);
      m_background->setBoxFit(BoxFit::Cover);
      auto name = *img;
      if (name.starts_with("https://")) {
        ImageCache::instance()->download(name, {}, "", [this, willSwitch, oldBg](CCImage* i, std::string) {
          auto texture = new CCTexture2D();
          texture->initWithImage(i);
          auto m = texture->getContentSize();
          m_background->getSprite()->setTexture(texture);
          m_background->getSprite()->setTextureRect(CCRect{0,0,m.width, m.height});
          m_background->refreshScaling();

          if (willSwitch) {
            oldBg->runAction(CCSequence::createWithTwoActions(
              CCFadeOut::create(0.5f),
              CCRemoveSelf::create()
            ));
            m_background->runAction(CCFadeIn::create(0.5f));
          }
        });
      } else {
        auto spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
        m_background->getSprite()->setTexture(spriteFrame->getTexture());
        m_background->getSprite()->setTextureRect(spriteFrame->getRect());
        
        if (willSwitch) {
          oldBg->runAction(CCFadeOut::create(0.5f));
          m_background->runAction(CCFadeIn::create(0.5f));
        }
      }
      m_background->setPosition(CCNode::getContentSize()/2);
      m_background->setContentSize(CCNode::getContentSize());
    } else if (e->isCancelled()) {
      return;
    }
  });



  // set the task
  m_backgroundGetListener.setFilter(

  meow::runWithCallback([this](auto finish, auto prog, auto cancelled){
    if (cancelled()) {
      finish(meow::Cancel());
      log::warn("[Background]: main task got twitter'd");
      return;
    }

    int seasonalEndTime = Mod::get()->getSavedValue<int>("seasonal-end",0);
    if (
      seasonalEndTime < std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
      ).count()
    ) {
      log::info("[Background]: Seasonal backgrounds URLs not cached or expired. Fetching stuff rq");
      auto r = web::WebRequest();
      m_seasonalBgsListener.bind([this,finish](web::WebTask::Event* e){
        if (auto v = e->getValue()) {
            auto data = v->json().unwrap();
          // TODO: check back when the current seasonal background ends to see which payload is gonna be when theres none
          std::tm t = {};
          std::stringstream ss(data["ends_at"].asString().unwrap());
          ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
          Mod::get()->setSavedValue(
            "seasonal-end",
            std::chrono::duration_cast<std::chrono::seconds>(
              std::chrono::system_clock::from_time_t(std::mktime(&t)).time_since_epoch()
            ).count()
          );
          matjson::Value sbg = matjson::Value::array();
          for (auto& i : data["backgrounds"]) {
            sbg.push(i["url"].asString().unwrap());
          }

          Mod::get()->setSavedValue("seasonal-backgrounds",sbg);

          m_backgrounds = sbg;
          finish(roll());
        }
      });
      m_seasonalBgsListener.setFilter(r.get("https://osu.ppy.sh/api/v2/seasonal-backgrounds"));
    } else {
      log::info("[Background]: Seasonal backgrounds URLs cached. Skipping fetch");
      auto sbg = Mod::get()->getSavedValue<matjson::Value>("seasonal-backgrounds");
      m_backgrounds = sbg;
      finish(roll());
    }
  }, "<Background:BGImageGet>")
  );
  setOpacity(255);
  setBackgroundColor({0,0,0,0});
  setContentSize({100,100},Unit::Percent);
  addListener<NodeSizeUpdated>([this](NodeSizeUpdated* ev){
    //auto e = static_cast<NodeLayoutUpdate*>(ev);
    if (m_background) {
      auto s = this->CCLayer::getContentSize();
      m_background->setPosition(s/2);
      m_background->setContentSize(s);
    }
    return true;
  });
  return true;
}

std::string Background::roll() {
  std::vector<matjson::Value> out;
  while (true) {
    std::sample(
      m_backgrounds.begin(),
      m_backgrounds.end(),
      std::back_inserter(out),
      1,
      std::mt19937{std::random_device{}()}
    );
    std::string bg = out[0].asString().unwrap();
    if (!m_background) return bg;
    if (bg != m_backgroundName) {
      return bg;
    }
  }
}
void Background::switchBackground() {
  m_backgroundGetListener.setFilter(meow::immediate(roll()));
};
GDL_NS_END
