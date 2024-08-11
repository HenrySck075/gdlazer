#pragma once
#include "../../framework/screens/Screen.hpp"

class OsuGame;
#include "../OsuGame.hpp"

// 2
class OsuScreen : public Screen {
private:
    OsuGame* game;
public:
    void setGameInstance(OsuGame* instance) {
        game = instance;
    }
};