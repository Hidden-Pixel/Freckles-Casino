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

global_variable Texture2D BlankGreenTableTexture;
global_variable Vector2 TableVector2;

global_variable Texture2D CardSlotTexture;
global_variable Vector2 CardSlotVector2[9];

global_variable Texture2D BackOfCardTexture;
global_variable Vector2 CardVector2[9];

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
    Image tempImage = LoadImage("assets/textures/Background/BlankTableGreen.png");
    ImageResizeNN(&tempImage, GlobalWindowWidth, GlobalWindowHeight / 2);
    BlankGreenTableTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
    TableVector2.y = (GlobalWindowHeight - BlankGreenTableTexture.height);
    TableVector2.x = 0;

    int resizedWidth = ((BlankGreenTableTexture.width - 10) / 9);
    resizedWidth = (int)floor(((float)resizedWidth) * 0.60f);
    int resizedHeight = ((BlankGreenTableTexture.height - 30) / 2);
    resizedHeight = (int)floor(((float)resizedHeight) * 0.75f);

    tempImage = LoadImage("assets/textures/Background/CardSlot.png");
    ImageResizeNN(&tempImage, resizedWidth, resizedHeight);
    CardSlotTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    resizedWidth = (int)floor((float)resizedWidth * 0.75f);
    resizedHeight = (int)floor((float)resizedHeight * 0.85f);

    tempImage = LoadImage("assets/textures/Cards/BackOfCard/BackOfCard.png");
    ImageResizeNN(&tempImage, resizedWidth, resizedHeight);
    BackOfCardTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
}

void
UnloadTextures()
{
    UnloadTexture(BlankGreenTableTexture);
    UnloadTexture(CardSlotTexture);
    UnloadTexture(BackOfCardTexture);
}

void
ProcessInput()
{
    // TODO(martin): add support for inputs
}

void
Render()
{
    BeginDrawing();
    {
        ClearBackground(BLANK);
        DrawTexture(BlankGreenTableTexture, TableVector2.x, TableVector2.y, WHITE);
        DrawTexture(CardSlotTexture, TableVector2.x, TableVector2.y + 25, WHITE);
        DrawTexture(BackOfCardTexture, TableVector2.x + 10, TableVector2.y + 35, WHITE);
    }
    EndDrawing();
}
