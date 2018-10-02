#pragma once

#include <raylib.h>

namespace freckles {
    namespace game_screen {
        void
        Initialize(float screenWidth, float screenHeight, float localWidth, float localHeight);

        float
        LocalUnitsToScreen(float units);

        Vector2
        LocalToScreen(Vector2 local);

        float
        ScreenUnitScale();
    }
}


