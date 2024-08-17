#pragma once

#include "Container.hpp"

enum class FillDirection {
    Full,
    Vertical,
    Horizontal
};

class FillFlowContainer : public Container {
private:
    FillDirection direction;
public:
    static FillFlowContainer* create(FillDirection dir) {
        create_class(FillFlowContainer, init, dir);
    }
    bool init(FillDirection dir);
    void updateChildPosition();
};