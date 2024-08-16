#pragma once

class Screen;
#include "Screen.hpp"


// contrary to popular belief and myself this morning this does not inherit NodeEvent
struct ScreenTransitionEvent {
public:
    /// <summary>
    /// The <see cref="Screen"/> which has been transitioned from.
    /// </summary>
    Screen* Current;

    /// <summary>
    /// The <see cref="Screen"/> which has been transitioned to.
    /// </summary>
    Screen* Destination;
};