#include "Background.hpp"
#include <chrono>

bool Background::init() {
    std::chrono::seconds seasonalEndTime(Mod::get()->getSavedValue<int>("seasonal-end",0));
    if (
        seasonalEndTime < std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
        )
    ) {
        
    }

    return true;
}