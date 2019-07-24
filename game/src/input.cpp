/*
 *  input.c
 *
 */

#include <FC/defines.h>
#include <FC/input.h>
#include <FC/commands.h>

#include <stdbool.h>
#include <assert.h>
#include <FC/ai-behavior.h>

using namespace freckles;

void Init_Input_State(Game_Input_State *game_input_state)
{
    game_input_state->hold_cursor_index = 0;
    game_input_state->hold_cursor_index_max = 5;
    zero_array(game_input_state->hold_cursor_selects);
}

void ProcessInput(poker::Game* game_poker_state, Game_Scene_State* game_scene_state, Game_Input_State* game_input_state)
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

void ProcessGamePlayInput(poker::Game* game_poker_state, Game_Scene_State* game_scene_state, Game_Input_State* game_input_state)
{
    if (IsKeyPressed(KEY_SPACE) &&
        game_poker_state->poker_state == poker::PokerState::NotStarted)
    {
        game_poker_state->poker_state = poker::PokerState::Started;
    }
    if (game_poker_state->poker_state >= poker::PokerState::PlayerCardsDealt)
    {
        if (game_poker_state->poker_state == poker::PokerState::SelectHolds)
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

                assert(game_poker_state->on_cardhold_pressed);
                game_poker_state->on_cardhold_pressed(game_poker_state->player_hand[game_input_state->hold_cursor_index],                                     ,
                                     game_input_state->hold_cursor_selects[game_input_state->hold_cursor_index]);

            }
            // TODO(Alex): No idea if this makes sense for moving on. "Are you sure?" might be appropriate here.
            if (IsKeyPressed(KEY_SPACE))
            {
                assert(game_poker_state->on_cardhold_pressed);
                AI_FiveCardDraw_MakeHoldDecision(game_poker_state->dealer_hand);
                game_poker_state->on_cardhold_complete(game_poker_state->dealer_hand, poker::CardState::Hidden);
                game_poker_state->on_cardhold_complete(game_poker_state->player_hand, poker::CardState::Shown);
                game_poker_state->dealer_hand_type = poker::find_best_hand(game_poker_state->dealer_hand);
                game_poker_state->player_hand_type = poker::find_best_hand(game_poker_state->player_hand);
                game_input_state->hold_cursor_selects[0] = CURSOR_NONE;
                game_input_state->hold_cursor_selects[1] = CURSOR_NONE;
                game_input_state->hold_cursor_selects[2] = CURSOR_NONE;
                game_input_state->hold_cursor_selects[3] = CURSOR_NONE;
                game_input_state->hold_cursor_selects[4] = CURSOR_NONE;
                game_poker_state->poker_state = poker::PokerState::ExchangeCards;
            }
        }
        if (game_poker_state->poker_state == poker::PokerState::ExchangeCards) {
            if (IsKeyPressed(KEY_SPACE)) {
                poker::reveal_hand(game_poker_state->dealer_hand, 5);
                game_poker_state->poker_state = poker::PokerState::GameOver;
                game_poker_state->on_game_over(game_poker_state->player_hand_type,
                        game_poker_state->dealer_hand_type);
            }
        }

        if (game_poker_state->poker_state == poker::PokerState::GameOver) {
            if (IsKeyPressed(KEY_N)) {
                poker::start_new_round(*game_poker_state);
            }
        }
    }
    
    game_poker_state->on_state_change(*game_poker_state);
}

void
ProcessTitleScreenInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state, Game_Input_State *game_input_state)
{
    static bool confirmPressed = false;
    if (IsKeyPressed(KEY_S) && confirmPressed == true)
    {
        confirmPressed = false;
    }
    if (IsKeyDown(KEY_S) && confirmPressed == false)
    {
        game_scene_state->current_scene = Scene_MainPokerTable;
    }
}
