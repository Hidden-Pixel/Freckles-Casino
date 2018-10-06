/*
 *  main.c
 *
 */

#include <FC/game-screen.h>
#include <FC/sprite-animation.h>
#include <FC/poker.h>
#include <FC/defines.h>

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
global_variable int GlobalFrameCount = 0;
global_variable int GlobalTargetFPS = 60;

global_variable Texture2D BlankGreenTableTexture;
global_variable Texture2D CardSlotTexture;
global_variable Texture2D BackOfCardTexture;
global_variable Texture2D ScoreFrameTexture;

// TODO: Load face cards
global_variable Texture2D CardTextures[52];

global_variable Vector2 CardAreaLeft;
global_variable Vector2 CardAreaRight;
global_variable Vector2 CardAreaCenter;
global_variable Vector2 TableVector2;
global_variable Vector2 ChancesBillboardArea;

global_variable Texture2D MrFrecklesSpritesheets[5];
global_variable SpriteAnimation MrFrecklesSpriteAnimation[5];
global_variable bool GameStarted = false;

const char* CreditsText = "CREDITS";
const char* JackpotText = "JACKPOT";
const char* BetText     = "BET";

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
    GameScreen_Init(GlobalWindowWidth, GlobalWindowHeight, GAME_WIDTH, GAME_HEIGHT);
    Poker_Init();
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
    ImageResizeNN(&tempImage, GlobalWindowWidth, GlobalWindowHeight / 2.0f);
    BlankGreenTableTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
    TableVector2.y = (GlobalWindowHeight - BlankGreenTableTexture.height);
    TableVector2.x = 0;
    CardAreaLeft.x = TableVector2.x + GameScreen_ScreenToLocalUnits(10.0f);
    CardAreaLeft.y = TableVector2.y + GameScreen_ScreenToLocalUnits(25.0f);

    tempImage = LoadImage("assets/textures/Background/CardSlot.png");

    Vector2 image_vector = { tempImage.width * 2.5f, tempImage.height * 2.5f };
    image_vector = Vector2Scale(image_vector, GameScreen_ScreenUnitScale());
    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);

    CardSlotTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    CardAreaRight.x = GlobalWindowWidth - GameScreen_ScreenToLocalUnits(15.0f) - 2.0f * CardSlotTexture.width;
    CardAreaRight.y = CardAreaLeft.y;


    // Distance formula for space between the two areas.
    float center_space = CardAreaRight.x - (CardAreaLeft.x + 2.0f * CardSlotTexture.width);

    // Find padding to evenly space on each side
    // We want to fit 5 cards in the space with two blank areas
    // f(x) = ((avail_space/size - x) * size) / 2
    float center_padding = (((center_space / CardSlotTexture.width) - 5.0f) * CardSlotTexture.width) / 2.0f;

    CardAreaCenter.x = (CardAreaLeft.x + 2.0f * CardSlotTexture.width) + center_padding;
    CardAreaCenter.y = CardAreaLeft.y + CardSlotTexture.height - GameScreen_ScreenToLocalUnits(3.0f);

    tempImage = LoadImage("assets/textures/Cards/BackOfCard/BackOfCard.png");

    image_vector.x = tempImage.width * 2.0f;
    image_vector.y = tempImage.height * 2.0f;
    image_vector = Vector2Scale(image_vector, GameScreen_ScreenUnitScale());

    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);
    BackOfCardTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/textures/Background/BrassPlate.png");
    image_vector.x = (CardSlotTexture.width * 2.0f) + GameScreen_ScreenToLocalUnits(1.0f);
    image_vector.y = CardSlotTexture.height;

    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);
    ScoreFrameTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/Idle.png");
    image_vector.x = tempImage.width * 2.0f;
    image_vector.y = tempImage.height * 2.0f;
    image_vector = Vector2Scale(image_vector, GameScreen_ScreenUnitScale());

    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);
    MrFrecklesSpritesheets[0] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
    MrFrecklesSpriteAnimation[0] = (SpriteAnimation)
    {
        .totalFrames            = 4,
        .totalVerticalFrames    = 2,
        .totalHorizontalFrames  = 2,
        .frameSpeed             = 2,
        //.frameOrder             = SpritesheetPosition.Horizontal | SpritesheetPosition.Vertical,
    };
}

void
UnloadTextures()
{
    UnloadTexture(BlankGreenTableTexture);
    UnloadTexture(CardSlotTexture);
    UnloadTexture(BackOfCardTexture);
    UnloadTexture(ScoreFrameTexture);
    // TODO(nick): replace with for loop ...
    UnloadTexture(MrFrecklesSpritesheets[0]);
}

void
ProcessInput()
{
    if (IsKeyDown(KEY_SPACE) && !GameStarted) {
        Poker_StartNewRound();
        GameStarted = true;
    }
}

void
Render()
{
    BeginDrawing();
    {
        ClearBackground(BLANK);
        DrawTexture(BlankGreenTableTexture, TableVector2.x, TableVector2.y, WHITE);

        Vector2 leftArea = { .x = CardAreaLeft.x + GameScreen_ScreenToLocalUnits(4.f),
                             .y = CardAreaLeft.y + GameScreen_ScreenToLocalUnits(5.f) };
        Vector2 rightArea = { .x = CardAreaRight.x + GameScreen_ScreenToLocalUnits(4.f),
                              .y = CardAreaRight.y + GameScreen_ScreenToLocalUnits(5.f) };
        Vector2 centerArea = { .x = CardAreaCenter.x + GameScreen_ScreenToLocalUnits(4.f),
                               .y = CardAreaCenter.y + GameScreen_ScreenToLocalUnits(5.f) };

        DrawHorizontalCardArea(CardSlotTexture, CardAreaLeft, 2, CardSlotTexture.width);
        DrawHorizontalCardArea(CardSlotTexture, CardAreaRight, 2, CardSlotTexture.width);
        DrawHorizontalCardArea(CardSlotTexture, CardAreaCenter, 5, CardSlotTexture.width);
        DrawHorizontalCardArea(BackOfCardTexture, leftArea, 2, CardSlotTexture.width);
        DrawHorizontalCardArea(BackOfCardTexture, rightArea, 2, CardSlotTexture.width);
        DrawHorizontalCardArea(BackOfCardTexture, centerArea, 5, CardSlotTexture.width);

        DrawTexture(ScoreFrameTexture, CardAreaLeft.x, CardAreaLeft.y + CardSlotTexture.height, WHITE);
        DrawTexture(ScoreFrameTexture, CardAreaRight.x, CardAreaLeft.y + CardSlotTexture.height, WHITE);

        DrawAnimationFrame(&MrFrecklesSpritesheets[0], &MrFrecklesSpriteAnimation[0], &centerArea, GlobalTargetFPS);
    }

    EndDrawing();
    GlobalFrameCount++;
}

void
DrawHorizontalCardArea(Texture2D texture, Vector2 area, int card_count, float x_shift)
{
    for (int i = 0; i < card_count; ++i) 
    {
        DrawTexture(texture, area.x + (i * x_shift), area.y, WHITE);
    }
}
