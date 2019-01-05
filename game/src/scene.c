/*
 *  scene.c
 *
 */

#include <FC/scene.h>

Game_Scene_State
Init_Game_Scene_State()
{
    return (Game_Scene_State)
    {
        .current_scene = Scene_LogoScreen,
    };
}
