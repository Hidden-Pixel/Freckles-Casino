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

static int card_cursor_index = 0;

void process_game_play_input(poker::Game& game_poker_state, Game_Scene_State* game_scene_state, int max_index)
{
    if (IsKeyPressed(KEY_SPACE) &&
            game_poker_state.poker_state == poker::PokerState::NotStarted)
    {
        poker::change_poker_state(game_poker_state, poker::PokerState::Started);
    }
    if (game_poker_state.poker_state >= poker::PokerState::PlayerCardsDealt)
    {
        if (game_poker_state.poker_state == poker::PokerState::SelectHolds)
        {
            if (IsKeyPressed(KEY_RIGHT))
            {
                if (card_cursor_index >= max_index)
                {
                    auto old_index = card_cursor_index;
                    card_cursor_index = 0;
                    game_poker_state.on_cursor_change(game_poker_state.player_hand[old_index], game_poker_state.player_hand[card_cursor_index]);
                }
            }
            if (IsKeyPressed(KEY_LEFT))
            {
                auto old_index = card_cursor_index;
                card_cursor_index--;
                if (card_cursor_index < 0)
                {
                    card_cursor_index = (max_index - 1);
                }
                game_poker_state.on_cursor_change(game_poker_state.player_hand[old_index], game_poker_state.player_hand[card_cursor_index]);
            }
            if (IsKeyPressed(KEY_H))
            {
                assert(game_poker_state.on_cardhold_pressed);
                game_poker_state.on_cardhold_pressed(game_poker_state.player_hand[card_cursor_index]);
            }
            // TODO(Alex): No idea if this makes sense for moving on. "Are you sure?" might be appropriate here.
            if (IsKeyPressed(KEY_SPACE))
            {
                assert(game_poker_state.on_cardhold_pressed);
                AI_FiveCardDraw_MakeHoldDecision(game_poker_state.dealer_hand);
                game_poker_state.on_cardhold_complete(game_poker_state.dealer_hand, poker::CardState::Hidden);
                game_poker_state.on_cardhold_complete(game_poker_state.player_hand, poker::CardState::Shown);
                game_poker_state.dealer_hand_type = poker::find_best_hand(game_poker_state.dealer_hand);
                game_poker_state.player_hand_type = poker::find_best_hand(game_poker_state.player_hand);
                poker::change_poker_state(game_poker_state, poker::PokerState::ExchangeCards);
            }
        }
        if (game_poker_state.poker_state == poker::PokerState::ExchangeCards) {
            if (IsKeyPressed(KEY_SPACE)) {
                poker::reveal_hand(game_poker_state.dealer_hand);
                poker::change_poker_state(game_poker_state, poker::PokerState::GameOver);
                game_poker_state.on_game_over(game_poker_state.player_hand_type,
                        game_poker_state.dealer_hand_type);
            }
        }

        if (game_poker_state.poker_state == poker::PokerState::GameOver) {
            if (IsKeyPressed(KEY_N)) {
                poker::start_new_round(game_poker_state);
            }
        }
    }
}

void process_title_screen_input(poker::Game& game_poker_state, Game_Scene_State* game_scene_state)
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

void freckles::input::update(poker::Game& game_poker_state, Game_Scene_State* game_scene_state)
{
    switch (game_scene_state->current_scene)
    {
        case Scene_MainPokerTable:
        {
            process_game_play_input(game_poker_state, game_scene_state, 5);
        } break;

        case Scene_TitleScreen:
        {
            process_title_screen_input(game_poker_state, game_scene_state);
        } break;
    }
}
