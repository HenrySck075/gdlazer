#include "Background.hpp"
#include "Geode/utils/web.hpp"
#include <chrono>
#include <matjson.hpp>

bool Background::init() {
    Container::init();
    m_background = CCResizableSprite::create();

    m_backgroundGetListener.bind([this](meow::Event* e){
        if (auto img = e->getValue()) {
            if (img->starts_with("https://")) {
                auto texture = new CCTexture2D();
                ImageCache::instance()->download(*img, {}, "", [texture,this](CCImage* i, std::string) {
                    texture->initWithImage(i);
                    m_background->getSprite()->setTexture(texture);
                });
            } else {
                m_background = CCResizableSprite::createWithSpriteFrameName(img->c_str());
            }
            m_background->preserveRatio(true);
        } else if (e->isCancelled()) {
            return;
        }
    });

    addListener("nodeLayoutUpdate", [this](NodeEvent* ev){
        auto e = static_cast<NodeUIEvent*>(ev);
        if (e->type == NodeUIEventType::Size) {
            auto s = getRealContentSize();
            m_background->setPosition(s/2);
            m_background->setContentSize(s);
        }
    });

    // set the task
    m_backgroundGetListener.setFilter(

    meow::runWithCallback([this](auto finish, auto prog, auto cancelled){
        if (cancelled) finish(meow::Cancel());

        std::chrono::seconds seasonalEndTime(Mod::get()->getSavedValue<int>("seasonal-end",0));
        if (
            seasonalEndTime < std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
            )
        ) {
            auto r = web::WebRequest();
            m_seasonalBgsListener.bind([this,finish](web::WebTask::Event* e){
                if (auto v = e->getValue()) {
                    auto data = v->json()->as_object();
                    // TODO: check back when the current seasonal background ends to see which payload is gonna be when theres none
                    std::tm t = {};
                    std::stringstream ss(data["ends_at"].as_string());
                    ss >> std::get_time(&t, "%Y-%M-%DT%H:%M:%S");
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
            finish(roll(Mod::get()->getSavedValue<matjson::Array>("seasonal-backgrounds")));
        }
    }, "<Background:BGImageGet>")

    );
    return true;
}

std::string Background::roll(matjson::Array images) {
    return images[(int)(randomFloat()*images.size())].as_string();
}
