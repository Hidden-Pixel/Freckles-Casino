/*
 *  main.c
 *
 */

#include "raylib.h"
#include "raymath.h"
#include <FC/entities.h>

#define global_variable static

// Our actual screen size
#define WIDTH 1280
#define HEIGHT 720

// Our target game size
// This gives us 240000 (240p) units to work with
// Classic old-school resolution.
#define GAME_WIDTH 600
#define GAME_HEIGHT 400

global_variable int GlobalWindowWidth   = WIDTH;
global_variable int GlobalWindowHeight  = HEIGHT;
global_variable const char *GlobalWindowTitle = "Mr. FC Casino";
global_variable unsigned char GlobalRunning = 1;
global_variable int GlobalTargetFPS = 60;

global_variable Texture2D BlankGreenTableTexture;
global_variable Vector2 TableVector2;

global_variable Texture2D CardSlotTexture;
global_variable Vector2 CardSlotVector2[9];

global_variable Texture2D BackOfCardTexture;
global_variable Vector2 CardVector2[9];

global_variable float GameScreenUnit = (float)(WIDTH * HEIGHT) / (GAME_WIDTH * GAME_HEIGHT) ;

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

    tempImage = LoadImage("assets/textures/Background/CardSlot.png");

    Vector2 image_vector = { tempImage.width * 1.25f, tempImage.height * 1.25f };
    image_vector = Vector2Scale(image_vector, GameScreenUnit);
    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);

    CardSlotTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/textures/Cards/BackOfCard/BackOfCard.png");

    image_vector.x = tempImage.width;
    image_vector.y = tempImage.height;
    image_vector = Vector2Scale(image_vector, GameScreenUnit);

    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);
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
        Vector2 card_slot_loc = { TableVector2.x + 4 * GameScreenUnit, TableVector2.y + 8 * GameScreenUnit};
        ClearBackground(BLANK);
        DrawTexture(BlankGreenTableTexture, TableVector2.x, TableVector2.y, WHITE);
        DrawTexture(CardSlotTexture, card_slot_loc.x, card_slot_loc.y, WHITE);
        DrawTexture(BackOfCardTexture, card_slot_loc.x + CardSlotTexture.width * .10, card_slot_loc.y + CardSlotTexture.height * .125, WHITE);
    }
    EndDrawing();
}
