#pragma once

#include "../../../../external/aubio/aubio.h"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class BeatDetector {
private:
    int winsize = 1024;
    int hopsize = winsize/4;
    int samplerate = 0;
    static BeatDetector* instance;
    aubio_onset_t* detector;
    FMODAudioEngine* engine;
    FMOD::System* system;

    aubio_source_t* sauce;

    CCNode* dummyNode;
public:
    BeatDetector() {
        detector = new_aubio_onset("default",winsize,hopsize,samplerate);
    }
    ~BeatDetector() {
        del_aubio_onset(detector);
    };
    static BeatDetector* instance() {
        if (instance == nullptr) {
            instance = new BeatDetector();
        }
        return instance;
    };

    void load();
    void update();
    bool isPlaying() { return engine->isMusicPlaying(1); }
};