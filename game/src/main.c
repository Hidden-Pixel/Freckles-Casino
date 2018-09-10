/*
 *  main.c
 *
 */

#include "raylib.h"
#include "raymath.h"

#define global_variable static

#define WIDTH 1280
#define HEIGHT 720

global_variable int GlobalWindowWidth   = WIDTH;
global_variable int GlobalWindowHeight  = HEIGHT;
global_variable char *GlobalWindowTitle = "Mr. Freckles Casino";
global_variable unsigned char GlobalRunning = 1;
global_variable int GlobalTargetFPS = 60;

global_variable Texture2D TestTexture1;
global_variable int X1 = WIDTH / 2;
global_variable int Y1 = HEIGHT / 2;

global_variable Texture2D TestTexture2;

void
LoadTextures();

void
UnloadTextures();

void
ProcessInput();

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
            ProcessInput();
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
ProcessInput()
{
    // TODO(martin): add support for mouse click on cards
}

void
Render()
{
    BeginDrawing();
    {
        ClearBackground(GREEN);
        DrawTexture(TestTexture1, X1, Y1, WHITE);
        //DrawTexture(TestTexture2, GlobalWindowWidth / 3, GlobalWindowHeight / 2, WHITE);
    }
    EndDrawing();
}
