/*
 *  scene.h
 *
 */

#pragma once

typedef enum _scene
{
    Scene_LogoScreen        = 0x00,
    Scene_TitleScreen       = 0x01,
    Scene_MainPokerTable    = 0x02,
} Scene;

typedef struct _game_scene_state
{
    Scene current_scene;
} Game_Scene_State;

Game_Scene_State
Init_Game_Scene_State();
