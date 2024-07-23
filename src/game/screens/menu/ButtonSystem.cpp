#include "ButtonSystem.hpp"

void ButtonSystem::setOsuLogo(OsuLogo* logo) {
        this->logo = logo;
        /*
        if (this->logo != nullptr)
        {
            this->logo.Action = onOsuLogo;

            // osuLogo.SizeForFlow relies on loading to be complete.
            buttonArea.Flow.Position = ccp(WEDGE_WIDTH * 2 - (BUTTON_WIDTH + this->logo.SizeForFlow / 4), 0);

            updateLogoState();
        }
        else
        {
            // We should stop tracking as the facade is now out of scope.
          
          logoTrackingContainer.StopTracking();
        }
        */
    }
