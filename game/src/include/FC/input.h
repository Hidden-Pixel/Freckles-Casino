#pragma once

#include <FC/poker.h>
#include <FC/scene.h>
#include <FC/commands.h>

#include "raylib.h"

namespace freckles {
    namespace input {

        enum class CursorState {
            None = 0,
            Blinking = 1,
            Selected = 2,
        };


        void update(poker::Game& game_poker_state, Game_Scene_State* game_scene_state);
    }
}