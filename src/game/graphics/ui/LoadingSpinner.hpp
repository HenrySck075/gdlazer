#pragma once

#include "../../../framework/graphics/containers/VisibilityContainer.hpp"

class LoadingSpinner final: public VisibilityContainer {
    

    CCSprite* spinner;
public:
    const float TRANSITION_DURATION = 0.5;

    const float spin_duration = 0.9;
    
    bool init(bool boxed = true, bool invert = false);
    static LoadingSpinner* create(bool boxed = true, bool invert = false);
    void onOpen() override;
    void onClose() override;
};

