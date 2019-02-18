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
#ifndef DEBUG_FRECKLES
        .current_scene = Scene_LogoScreen,
#else
        .current_scene = Scene_TitleScreen,
#endif
    };
}
