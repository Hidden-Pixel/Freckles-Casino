/*
 *  main.c
 *
 */

#include "raylib.h"
#include "raymath.h"

#define global_variable static

global_variable int GlobalWindowWidth   = 1280;
global_variable int GlobalWindowHeight  = 720;
global_variable char *GlobalWindowTitle = "Mr. Freckles Casino";
global_variable unsigned char GlobalRunning = 1;
global_variable int GlobalTargetFPS = 60;

global_variable Texture2D TestTexture1;
global_variable Texture2D TestTexture2;

void
LoadTextures();

void
UnloadTextures();

void
Render();

int
main(void)
{
    InitWindow(GlobalWindowWidth, GlobalWindowHeight, GlobalWindowTitle);
    SetTargetFPS(GlobalTargetFPS);

    LoadTextures();

    if (IsWindowReady())
    {
        while (GlobalRunning)
        {
            Render();
            if (WindowShouldClose())
            {
                GlobalRunning = 0;
            }
        }
        CloseWindow();
    }

    UnloadTextures();

    return 0;
}

void
LoadTextures()
{
    Image tempImage = LoadImage("assets/Cards/BackOfCard/BackOfCard.png");
    ImageResizeNN(&tempImage, tempImage.width / 4, tempImage.height / 4);
    TestTexture1 = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/Cards/Hearts/2_Hearts.png");
    ImageResizeNN(&tempImage, tempImage.width / 4, tempImage.height / 4);
    TestTexture2 = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
}

void
UnloadTextures()
{
    UnloadTexture(TestTexture1);
    UnloadTexture(TestTexture2);

}

void
Render()
{
    BeginDrawing();
    {
        ClearBackground(GREEN);
        DrawTexture(TestTexture1, GlobalWindowWidth / 2, GlobalWindowHeight / 2, WHITE);
        // TODO(martin): render textures to the screen to the left of the above texture
        DrawTexture(TestTexture2, GlobalWindowWidth / 3, GlobalWindowHeight / 2, WHITE);
    }
    EndDrawing();
}
