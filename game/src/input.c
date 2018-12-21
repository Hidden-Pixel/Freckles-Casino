/*
 *  input.c
 *
 */

#include <FC/defines.h>
#include <FC/input.h>
#include <FC/commands.h>

#include <stdbool.h>
#include <assert.h>

void
Init_Input_State(Game_Input_State *game_input_state)
{
    game_input_state->hold_cursor_index = 0;
    game_input_state->hold_cursor_index_max = 5;
    zero_array(game_input_state->hold_cursor_selects);
}

void
ProcessInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state, Game_Input_State *game_input_state)
{
    switch (game_scene_state->current_scene)
    {
        case Scene_MainPokerTable:
        {
            ProcessGamePlayInput(game_poker_state, game_scene_state, game_input_state);
        } break;

        case Scene_TitleScreen:
        {
            ProcessTitleScreenInput(game_poker_state, game_scene_state, game_input_state);
        } break;
    }
}

void
ProcessGamePlayInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state, Game_Input_State *game_input_state)
{
    if (IsKeyPressed(KEY_SPACE) &&
        game_poker_state->poker_state == PokerState_NotStarted)
    {
        game_poker_state->poker_state = PokerState_Started;
    }
    if (game_poker_state->poker_state >= PokerState_PlayerCardsDealt)
    {
        if (game_poker_state->poker_state == PokerState_SelectHolds)
        {
            if (IsKeyPressed(KEY_RIGHT))
            {
                game_input_state->hold_cursor_index++;
                if (game_input_state->hold_cursor_index >= game_input_state->hold_cursor_index_max)
                {
                    game_input_state->hold_cursor_index = 0;
                }
            }
            if (IsKeyPressed(KEY_LEFT))
            {
                game_input_state->hold_cursor_index--;
                if (game_input_state->hold_cursor_index < 0)
                {
                    game_input_state->hold_cursor_index = (game_input_state->hold_cursor_index_max - 1);
                }
            }
            if (IsKeyPressed(KEY_H))
            {
                if (game_input_state->hold_cursor_selects[game_input_state->hold_cursor_index] == CURSOR_SELECTED)
                {
                    game_input_state->hold_cursor_selects[game_input_state->hold_cursor_index] = CURSOR_NONE;
                }
                else
                {
                    game_input_state->hold_cursor_selects[game_input_state->hold_cursor_index] = CURSOR_SELECTED;
                }

                assert(Command_OnCardHoldPressed);
                Command_OnCardHoldPressed(game_poker_state->player_hand,
                                     game_input_state->hold_cursor_index,
                                     game_input_state->hold_cursor_selects[game_input_state->hold_cursor_index]);

            }
            // TODO(Alex): No idea if this makes sense for moving on. "Are you sure?" might be appropriate here.
            if (IsKeyPressed(KEY_SPACE))
            {
                assert(Command_OnCardHoldComplete);
                Command_OnCardHoldComplete(game_poker_state->player_hand, 5);
                game_poker_state->poker_state = PokerState_ExchangeCards;
            }
        }
    }
    Poker_ProcessState(game_poker_state);
}

void
ProcessTitleScreenInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state, Game_Input_State *game_input_state)
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
