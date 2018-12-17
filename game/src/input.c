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
    switch (game_scene_state->current_scene)
    {
        case Scene_MainPokerTable:
        {
            ProcessGamePlayInput(game_poker_state, game_scene_state);
        } break;

        case Scene_TitleScreen:
        {
            ProcessTitleScreenInput(game_poker_state, game_scene_state);
        } break;
    }
}

void
ProcessGamePlayInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state)
{
    local_persist bool confirmPressed = false;

    if (IsKeyDown(KEY_SPACE) && confirmPressed == true)
    {
        confirmPressed = false;
    }
    if (IsKeyDown(KEY_SPACE) && confirmPressed == false &&
        game_poker_state->poker_state == PokerState_NotStarted)
    {
        game_poker_state->poker_state = PokerState_Started;
    }
    if (game_poker_state->poker_state >= PokerState_PlayerCardsDealt)
    {
        // TODO(nick):
        /*
        if (game_poker_state->poker_state == PokerState_CardBetting)
        {
            if (IsKeyDown(KEY_H) && confirmedPressed == true)
            {
                // TODO(nick): set card index to hold state
            }
            if (IsKeyDown(KEY_H) && confirmedPressed == false)
            {
                confirmedPressed = false;
            }
        }
        */
    }
    Poker_ProcessState(game_poker_state);
}

void
ProcessTitleScreenInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state)
{
    local_persist bool confirmPressed = false;
    if (IsKeyPressed(KEY_S) && confirmPressed == true)
    {
        confirmPressed = false;
    }
    if (IsKeyDown(KEY_S) && confirmPressed == false)
    {
        game_scene_state->current_scene = Scene_MainPokerTable;
    }
}
