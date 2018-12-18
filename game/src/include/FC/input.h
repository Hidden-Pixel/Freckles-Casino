/*
 *  input.h
 *
 */

#pragma once

#include <FC/poker.h>
#include <FC/scene.h>

#include "raylib.h"

typedef struct game_Input_State
{
    int hold_cursor_index;
    int hold_cursor_index_max;
} Game_Input_State;

void
Init_Input_State(Game_Input_State *game_input_state);

void
ProcessInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state, Game_Input_State *game_input_state);

void
ProcessGamePlayInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state, Game_Input_State *game_input_state);

void
ProcessTitleScreenInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state, Game_Input_State *game_input_state);
