/*
 *  input.c
 *
 */

#include <FC/defines.h>
#include <FC/input.h>

#include <stdbool.h>

void
ProcessInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state)
{
    local_persist bool confirmPressed = false;
    if (IsKeyPressed(KEY_S) && confirmPressed == true)
    {
        confirmPressed = false;
    }
    if (IsKeyDown(KEY_S) && confirmPressed == false && game_scene_state->current_scene == Scene_TitleScreen)
    {
        game_scene_state->current_scene = Scene_MainPokerTable;
    }

    if (game_scene_state->current_scene != Scene_TitleScreen)
    {
        if (IsKeyDown(KEY_SPACE) && confirmPressed == true)
        {
            confirmPressed = false;
        }
        if (IsKeyDown(KEY_SPACE) && confirmPressed == false && game_poker_state->poker_state == PokerState_NotStarted)
        {
            Poker_ProcessNewState(game_poker_state);
        }
    }
}
