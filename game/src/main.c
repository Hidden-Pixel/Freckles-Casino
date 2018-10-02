/*
 *  main.c
 *
 */

#include <FC/game-screen.h>
#include "raylib.h"
#include "raymath.h"

#define global_variable static

// Our actual screen size
#define WIDTH 1280
#define HEIGHT 800

// Our target game size
// This gives us 102240 (240p) units to work with
// Classic old-school resolution.
#define GAME_WIDTH 426
#define GAME_HEIGHT 240

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

global_variable Texture2D ScoreFrameTexture;

global_variable Vector2 CardAreaLeft;
global_variable Vector2 CardAreaRight;
global_variable Vector2 CardAreaCenter;
global_variable Vector2 ChancesBillboardArea;

const char* CreditsText = "CREDITS";
const char* JackpotText = "JACKPOT";
const char* BetText = "BET";


void
LoadTextures();

void
UnloadTextures();

void
ProcessInput();

void
Render();

void
DrawHorizontalCardArea(Texture2D texture, Vector2 area, int card_count, float x_shift);

int
main(void)
{
    using namespace freckles;

    game_screen::Initialize(GlobalWindowWidth, GlobalWindowHeight, GAME_WIDTH, GAME_HEIGHT);
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
    using namespace freckles;

    Image tempImage = LoadImage("assets/textures/Background/BlankTableGreen.png");
    ImageResizeNN(&tempImage, GlobalWindowWidth, GlobalWindowHeight / 2);
    BlankGreenTableTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
    TableVector2.y = (GlobalWindowHeight - BlankGreenTableTexture.height);
    TableVector2.x = 0;
    CardAreaLeft.x = TableVector2.x + game_screen::LocalUnitsToScreen(10.f);
    CardAreaLeft.y = TableVector2.y + game_screen::LocalUnitsToScreen(30.f);

    tempImage = LoadImage("assets/textures/Background/CardSlot.png");

    Vector2 image_vector = { tempImage.width * 2.5f, tempImage.height * 2.5f };
    image_vector = Vector2Scale(image_vector, game_screen::ScreenUnitScale());
    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);

    CardSlotTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    CardAreaRight.x = GlobalWindowWidth - game_screen::LocalUnitsToScreen(15.f) - 2 * CardSlotTexture.width;
    CardAreaRight.y = CardAreaLeft.y;

    float center_space = CardAreaRight.x - (CardAreaLeft.x + 2 * CardSlotTexture.width);
    float center_padding = (((center_space / CardSlotTexture.width) - 5) * CardSlotTexture.width) / 2;

    CardAreaCenter.x = (CardAreaLeft.x + 2 * CardSlotTexture.width) + center_padding;
    CardAreaCenter.y = CardAreaLeft.y + CardSlotTexture.height;

    tempImage = LoadImage("assets/textures/Cards/BackOfCard/BackOfCard.png");

    image_vector.x = tempImage.width * 2.f;
    image_vector.y = tempImage.height * 2.f;
    image_vector = Vector2Scale(image_vector, game_screen::ScreenUnitScale());

    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);
    BackOfCardTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/textures/Background/BrassPlate.png");
    image_vector.x = (CardSlotTexture.width * 2) + game_screen::LocalUnitsToScreen(1.f);
    image_vector.y = CardSlotTexture.height;

    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);
    ScoreFrameTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
}

void
UnloadTextures()
{
    UnloadTexture(BlankGreenTableTexture);
    UnloadTexture(CardSlotTexture);
    UnloadTexture(BackOfCardTexture);
    UnloadTexture(ScoreFrameTexture);
}

void
ProcessInput()
{
    // TODO(martin): add support for inputs
}

void
Render()
{
    using namespace freckles;

    BeginDrawing();
    {
        ClearBackground(BLANK);
        DrawTexture(BlankGreenTableTexture, TableVector2.x, TableVector2.y, WHITE);

        DrawHorizontalCardArea(CardSlotTexture, CardAreaLeft, 2, CardSlotTexture.width);
        DrawHorizontalCardArea(CardSlotTexture, CardAreaRight, 2, CardSlotTexture.width);
        DrawHorizontalCardArea(CardSlotTexture, CardAreaCenter, 5, CardSlotTexture.width);
        DrawHorizontalCardArea(BackOfCardTexture,
                Vector2 { CardAreaLeft.x + game_screen::LocalUnitsToScreen(4.f),
                          CardAreaLeft.y + game_screen::LocalUnitsToScreen(5.f) }, 2, CardSlotTexture.width);
        DrawHorizontalCardArea(BackOfCardTexture,
                     Vector2 { CardAreaRight.x + game_screen::LocalUnitsToScreen(4.f),
                               CardAreaRight.y + game_screen::LocalUnitsToScreen(5.f) }, 2, CardSlotTexture.width);
        DrawHorizontalCardArea(BackOfCardTexture,
                     Vector2 { CardAreaCenter.x + game_screen::LocalUnitsToScreen(4.f),
                               CardAreaCenter.y + game_screen::LocalUnitsToScreen(5.f) }, 5, CardSlotTexture.width);

        DrawTexture(ScoreFrameTexture, CardAreaLeft.x, CardAreaLeft.y + CardSlotTexture.height, WHITE);
        DrawTexture(ScoreFrameTexture, CardAreaRight.x, CardAreaLeft.y + CardSlotTexture.height, WHITE);
    }
    EndDrawing();
}

void
DrawHorizontalCardArea(Texture2D texture, Vector2 area, int card_count, float x_shift)
{
    for (int i = 0; i < card_count; ++i) {
        DrawTexture(texture, area.x + (i * x_shift), area.y, WHITE);
    }
}