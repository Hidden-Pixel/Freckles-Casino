/*
 *  input.h
 *
 */

#pragma once

#include <FC/poker.h>
#include <FC/scene.h>

#include "raylib.h"

void
ProcessInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state);

void
ProcessGamePlayInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state);

void
ProcessTitleScreenInput(Poker_Game *game_poker_state, Game_Scene_State *game_scene_state);
