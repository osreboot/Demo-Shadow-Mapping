#pragma once

#include "geo/vec2f.h"
#include "geo/vec2i.h"

namespace display {

    // Display lifecycle functions
    void initialize(); // Called to create the display
    void preUpdate(); // Called before rendering game elements
    void postUpdate(); // Called after rendering game elements
    void close(); // Called to destroy the display

    // Returns whether the user has requested the display close
    bool isExiting();

    // Returns the size of the display
    const vec2i& getSize();

    // Returns the position of the mouse cursor
    const vec2f& getCursor();

    // Returns true if an input-related event has been triggered on this particular tick
    bool hasEventMousePress();
    bool hasEventMouseRelease();
    bool hasEventKeyPress(int key);
    bool hasEventKeyRelease(int key);

}
