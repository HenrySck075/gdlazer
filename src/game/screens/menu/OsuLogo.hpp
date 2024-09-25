#pragma once

/* 
#include "../../graphics/containers/beatsync/BeatDetector.hpp"
#include "../../graphics/containers/BeatSyncedContainer.hpp"
#include "../../graphics/containers/beatsync/BeatEvent.hpp"
*/
#include "../../../framework/graphics/containers/ClickableContainer.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class OsuLogo : public ClickableContainer {
private:
    //BeatDetector* instance;
    int beatCount = 0; // TODO: its not guaranteed to be always 0, but we mute the music anyways so
public:  
    void onMouseEnter() override {};
    void onMouseExit() override {};
    //void onBeat(float delta) override;
    void onMouseUp(MouseEvent* event) override {};
    void onMouseDown(MouseEvent* event) override {};
    void onClick(MouseEvent* e) override {};
    void update(float delta);

    static OsuLogo* create() {
        OsuLogo *ret = new OsuLogo();
        if (ret && ret->init()) {
            ret->ClickableContainer::autorelease();
        } else {
            if (ret) {
                delete (ret);
            }
        }
        return ret;
    }
    bool init();

};
