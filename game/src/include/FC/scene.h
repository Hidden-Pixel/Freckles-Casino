/*
 *  scene.h
 *
 */

#pragma once

typedef enum _scene
{
    Scene_TitleScreen       = 0x00,
    Scene_MainPokerTable    = 0x01,
} Scene;

typedef struct _game_scene_state
{
    Scene current_scene;
} Game_Scene_State;

Game_Scene_State
Init_Game_Scene_State();
