#pragma once
#include "../../framework/screens/Screen.hpp"

class Playground : public Screen {
public:
    bool init() {
        if (!Screen::init()) return false;
        setRadius(16);
        return true;
    }
};