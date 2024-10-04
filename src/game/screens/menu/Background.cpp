#include "Background.hpp"
#include "Geode/utils/web.hpp"
#include <chrono>
#include <matjson.hpp>
#include <random>

bool Background::init() {
    Container::init();
    m_background = CCResizableSprite::create();
    addChild(m_background);
    m_backgroundGetListener.bind([this](meow::Event* e){
        if (auto img = e->getValue()) {
            m_background->setBoxFit(BoxFit::Cover);
            auto name = *img;
            if (name.starts_with("https://")) {
                ImageCache::instance()->download(name, {}, "", [this](CCImage* i, std::string) {
                    auto texture = new CCTexture2D();
                    texture->initWithImage(i);
                    auto m = texture->getContentSize();
                    m_background->getSprite()->setTexture(texture);
                    m_background->getSprite()->setTextureRect(CCRect{0,0,m.width, m.height});
                    m_background->refreshScaling();
                });
            } else {
                auto spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
                m_background->getSprite()->setTexture(spriteFrame->getTexture());
                m_background->getSprite()->setTextureRect(spriteFrame->getRect());
            }
            m_background->setContentSize(getContentSize());
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
                    auto data = v->json()->as_object();
                    // TODO: check back when the current seasonal background ends to see which payload is gonna be when theres none
                    std::tm t = {};
                    std::stringstream ss(data["ends_at"].as_string());
                    ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
                    Mod::get()->setSavedValue(
                        "seasonal-end",
                        std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::system_clock::from_time_t(std::mktime(&t)).time_since_epoch()
                        ).count()
                    );
                    matjson::Array sbg;
                    for (auto i : data["backgrounds"].as_array()) {
                        sbg.push_back(i["url"].as_string());
                    }

                    Mod::get()->setSavedValue("seasonal-backgrounds",sbg);

                    finish(roll(sbg));
                }
            });
            m_seasonalBgsListener.setFilter(r.get("https://osu.ppy.sh/api/v2/seasonal-backgrounds"));
        } else {
            log::info("[Background]: Seasonal backgrounds URLs cached. Skipping fetch");
            auto sbg = Mod::get()->getSavedValue<matjson::Array>("seasonal-backgrounds");
            finish(roll(sbg));
        }
    }, "<Background:BGImageGet>")
    );
    setCascadeOpacityEnabled(true);
    setOpacity(255);
    setColor({0,0,0});
    setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
    addListener("nodeLayoutUpdate", [this](NodeEvent* ev){
        //auto e = static_cast<NodeLayoutUpdate*>(ev);
        auto s = CCLayer::getContentSize();
        m_background->setPosition(s/2);
        m_background->setContentSize(s);
    });
    return true;
}

std::string Background::roll(matjson::Array images) {
    std::vector<matjson::Value> winner;
    std::sample(
        images.begin(),
        images.end(),
        std::back_inserter(winner),
        1,
        std::mt19937{std::random_device{}()}
    );
    return winner[0].as_string();
}
