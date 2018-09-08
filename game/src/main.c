/*
 *  main.c
 *
 */

#include "raylib.h"
#include "raymath.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define global_variable static

int main(void)
{
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    // regular update loop and lock frame rate
#endif
    return 0;
}
