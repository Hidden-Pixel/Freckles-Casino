/*
 *  main.c
 *
 */

#include <FC/game-screen.h>
#include <FC/sprite-animation.h>
#include <FC/poker.h>
#include <FC/defines.h>

#include <stdio.h>

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
global_variable Texture2D RedCurtainTexture;
global_variable Texture2D CardSlotTexture;
global_variable Texture2D BackOfCardTexture;
global_variable Texture2D ScoreFrameTexture;

global_variable Texture2D CardTextures[CardSuit_Count * CardFace_Count];
global_variable SpriteAnimation HighCardSpriteAnimation;

global_variable Vector2 RedCurtainVector2;
global_variable Vector2 CardAreaLeft;
global_variable Vector2 CardAreaRight;
global_variable Vector2 CardAreaCenter;
global_variable Vector2 TableVector2;
global_variable Vector2 TableAreaCenter;
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
LoadCardTexture(char *filePath, Texture2D *texture);

void
LoadCardsTextures(Texture2D CardTextures[52], Texture2D *BackOfCardTexture);

void
UnloadTextures();

void
ProcessInput();

void
Render();

void
DrawHorizontalCardArea(Texture2D texture, Vector2 area, int card_count, float x_shift);

void
HandleConfirmButtonPress(Poker_Game* game_state);

int
main(void)
{
    local_variable Poker_Game game_state;

    GameScreen_Init(GlobalWindowWidth, GlobalWindowHeight, GAME_WIDTH, GAME_HEIGHT);
    Poker_Init(&game_state);
    InitWindow(GlobalWindowWidth, GlobalWindowHeight, GlobalWindowTitle);
    SetTargetFPS(GlobalTargetFPS);

    LoadTextures();

    if (IsWindowReady())
    {
        while (GlobalRunning)
        {
            ProcessInput(&game_state);
            Render(&game_state);
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
LoadCardTexture(char *filePath, Texture2D *texture) 
{
    float cardScale = 2.0f;
    float vectorScale = 2.5f;
    Vector2 image_vector =
    { 
        .x = 0.0f,
        .y = 0.0f,
    };
    Image tempImage = LoadImage(filePath);
    image_vector.x = tempImage.width * cardScale;
    image_vector.y = tempImage.height * cardScale;
    image_vector = Vector2Scale(image_vector, GameScreen_ScreenUnitScale());
    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);
    *texture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
}

inline void
LoadCardsTextures(Texture2D CardTextures[52], Texture2D *BackOfCardTexture)
{
    // NOTE: 15x12
    LoadCardTexture("assets/textures/Cards/BackOfCard/BackOfCard.png", BackOfCardTexture);

    // Hearts
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/2Hearts.png", &CardTextures[0]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/3Hearts.png", &CardTextures[1]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/4Hearts.png", &CardTextures[2]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/5Hearts.png", &CardTextures[3]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/6Hearts.png", &CardTextures[4]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/7Hearts.png", &CardTextures[5]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/8Hearts.png", &CardTextures[6]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/9Hearts.png", &CardTextures[7]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/10Hearts.png", &CardTextures[8]);
    LoadCardTexture("assets/textures/Cards/Hearts/Spritesheets/JackHearts.png", &CardTextures[9]);
    LoadCardTexture("assets/textures/Cards/Hearts/Spritesheets/QueenHearts.png", &CardTextures[10]);
    LoadCardTexture("assets/textures/Cards/Hearts/Spritesheets/KingHearts.png", &CardTextures[11]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/AceHearts.png", &CardTextures[12]);


    // Clubs
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/2Clubs.png", &CardTextures[13]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/3Clubs.png", &CardTextures[14]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/4Clubs.png", &CardTextures[15]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/5Clubs.png", &CardTextures[16]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/6Clubs.png", &CardTextures[17]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/7Clubs.png", &CardTextures[18]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/8Clubs.png", &CardTextures[19]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/9Clubs.png", &CardTextures[20]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/10Clubs.png", &CardTextures[21]);
    LoadCardTexture("assets/textures/Cards/Clubs/Spritesheets/JackClubs.png", &CardTextures[22]);
    LoadCardTexture("assets/textures/Cards/Clubs/Spritesheets/QueenClubs.png", &CardTextures[23]);
    LoadCardTexture("assets/textures/Cards/Clubs/Spritesheets/KingClubs.png", &CardTextures[24]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/AceClubs.png", &CardTextures[25]);

    //Diamonds
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/2Dia.png", &CardTextures[26]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/3Dia.png", &CardTextures[27]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/4Dia.png", &CardTextures[28]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/5Dia.png", &CardTextures[29]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/6Dia.png", &CardTextures[30]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/7Dia.png", &CardTextures[31]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/8Dia.png", &CardTextures[32]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/9Dia.png", &CardTextures[33]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/10Dia.png", &CardTextures[34]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Spritesheets/JackDia.png", &CardTextures[35]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Spritesheets/QueenDia.png", &CardTextures[36]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Spritesheets/KingDia.png", &CardTextures[37]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/AceDia.png", &CardTextures[38]);

    // Spades
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/2Spades.png", &CardTextures[39]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/3Spades.png", &CardTextures[40]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/4Spades.png", &CardTextures[41]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/5Spades.png", &CardTextures[42]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/6Spades.png", &CardTextures[43]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/7Spades.png", &CardTextures[44]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/8Spades.png", &CardTextures[45]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/9Spades.png", &CardTextures[46]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/10Spades.png", &CardTextures[47]);
    LoadCardTexture("assets/textures/Cards/Spades/Spritesheets/JackSpades.png", &CardTextures[48]);
    LoadCardTexture("assets/textures/Cards/Spades/Spritesheets/QueenSpades.png", &CardTextures[49]);
    LoadCardTexture("assets/textures/Cards/Spades/Spritesheets/KingSpades.png", &CardTextures[50]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/AceSpades.png", &CardTextures[51]);

    HighCardSpriteAnimation = (SpriteAnimation)
    {
        .totalFrames            = 12,
        .totalVerticalFrames    = 4,
        .totalHorizontalFrames  = 4,
        .frameSpeed             = 2,
    };
}

void
LoadTextures()
{
    // NOTE: load BlankTableGreen texture
    Image tempImage = LoadImage("assets/textures/Background/BlankTableGreen.png");
    ImageResizeNN(&tempImage, GlobalWindowWidth, GlobalWindowHeight / 2.0f);
    BlankGreenTableTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
    TableVector2.y = (GlobalWindowHeight - BlankGreenTableTexture.height);
    TableVector2.x = 0;
    TableAreaCenter.y = TableVector2.y;
    TableAreaCenter.x = (BlankGreenTableTexture.width / 2.0f);
    CardAreaLeft.x = TableVector2.x + GameScreen_LocalUnitsToScreen(10.0f);
    CardAreaLeft.y = TableVector2.y + GameScreen_LocalUnitsToScreen(25.0f);

    // NOTE: load Red Curtain Texture
    tempImage = LoadImage("assets/textures/Background/Curtain.png");
    ImageResizeNN(&tempImage, GlobalWindowWidth, GlobalWindowHeight / 2.0f);
    RedCurtainTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
    RedCurtainVector2.x = 0;
    RedCurtainVector2.x = 0;


    // NOTE: load CardSlot texture 15x20
    tempImage = LoadImage("assets/textures/Background/CardSlot.png");
    Vector2 image_vector = { tempImage.width * 2.5f, tempImage.height * 2.5f };
    image_vector = Vector2Scale(image_vector, GameScreen_ScreenUnitScale());
    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);
    CardSlotTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
    CardAreaRight.x = GlobalWindowWidth - GameScreen_LocalUnitsToScreen(15.0f) - 2.0f * CardSlotTexture.width;
    CardAreaRight.y = CardAreaLeft.y;


    // Distance formula for space between the two areas.
    float center_space = CardAreaRight.x - (CardAreaLeft.x + 2.0f * CardSlotTexture.width);

    // Find padding to evenly space on each side
    // We want to fit 5 cards in the space with two blank areas
    // f(x) = ((avail_space/size - x) * size) / 2
    float center_padding = (((center_space / CardSlotTexture.width) - 5.0f) * CardSlotTexture.width) / 2.0f;

    CardAreaCenter.x = (CardAreaLeft.x + 2.0f * CardSlotTexture.width) + center_padding;
    CardAreaCenter.y = CardAreaLeft.y + CardSlotTexture.height - GameScreen_LocalUnitsToScreen(3.0f);

    tempImage = LoadImage("assets/textures/Background/BrassPlate.png");
    image_vector.x = (CardSlotTexture.width * 2.0f) + GameScreen_LocalUnitsToScreen(1.0f);
    image_vector.y = CardSlotTexture.height;

    ImageResizeNN(&tempImage, image_vector.x, image_vector.y);
    ScoreFrameTexture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/Idle.png");
    image_vector.x = (tempImage.width * 3.0f) + GameScreen_LocalUnitsToScreen(1.0f);
    image_vector.y = (tempImage.height * 3.0f) + GameScreen_LocalUnitsToScreen(1.0f);
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

    LoadCardsTextures(CardTextures, &BackOfCardTexture);
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
ProcessInput(Poker_Game* game_state)
{
    static bool confirmPressed = false;

    if (IsKeyDown(KEY_SPACE) && confirmPressed == false)
    {
        confirmPressed = true;
        HandleConfirmButtonPress(game_state);
    }
    
    if (IsKeyUp(KEY_SPACE) && confirmPressed == true)
    {
        confirmPressed = false;
    }
}

void
Render(Poker_Game* game_state)
{
    BeginDrawing();
    {
        ClearBackground(BLACK);
        DrawTexture(RedCurtainTexture, RedCurtainVector2.x, RedCurtainVector2.y, WHITE);
        DrawTexture(BlankGreenTableTexture, TableVector2.x, TableVector2.y, WHITE);
        Vector2 leftArea =
        { 
            .x = CardAreaLeft.x + GameScreen_LocalUnitsToScreen(4.0f),
            .y = CardAreaLeft.y + GameScreen_LocalUnitsToScreen(5.0f),
        };
        Vector2 rightArea =
        {
            .x = CardAreaRight.x + GameScreen_LocalUnitsToScreen(4.f),
            .y = CardAreaRight.y + GameScreen_LocalUnitsToScreen(5.f),
        };
        Vector2 centerArea =
        {
            .x = CardAreaCenter.x + GameScreen_LocalUnitsToScreen(4.0f),
            .y = CardAreaCenter.y + GameScreen_LocalUnitsToScreen(5.0f),
        };
        DrawHorizontalCardArea(CardSlotTexture, CardAreaLeft, 2, CardSlotTexture.width);
        DrawHorizontalCardArea(CardSlotTexture, CardAreaRight, 2, CardSlotTexture.width);
        DrawHorizontalCardArea(CardSlotTexture, CardAreaCenter, 5, CardSlotTexture.width);

        // NOTE: A tiny optimization would be parallel arrays for cards and textures.
        for (int i = 0; i < 2; ++i)
        {
            float shift = CardSlotTexture.width * i;

            if (game_state->player_hand[i].state == CardState_Hidden)
            {
                DrawTexture(BackOfCardTexture, leftArea.x + shift, leftArea.y, WHITE);
            }
            else if (game_state->player_hand[i].state == CardState_Shown)
            {
                DrawFaceCard(game_state->player_hand[i], leftArea.x + shift, leftArea.y);
            }

            if (game_state->dealer_hand[i].state == CardState_Hidden)
            {
                DrawTexture(BackOfCardTexture, rightArea.x + shift, rightArea.y, WHITE);
            }
            else if (game_state->dealer_hand[i].state == CardState_Shown)
            {
                DrawFaceCard(game_state->dealer_hand[i], rightArea.x + shift, rightArea.y);
            }
        }

        for (int i = 0; i < 5; ++i)
        {
            float shift = CardSlotTexture.width * i;

            if (game_state->house_hand[i].state == CardState_Hidden)
            {
                DrawTexture(BackOfCardTexture, centerArea.x + shift, centerArea.y, WHITE);
            }
            else if (game_state->house_hand[i].state == CardState_Shown)
            {
                DrawFaceCard(game_state->house_hand[i], centerArea.x + shift, centerArea.y);
            }
        }

        DrawTexture(ScoreFrameTexture, CardAreaLeft.x, CardAreaLeft.y + CardSlotTexture.height, WHITE);
        DrawTexture(ScoreFrameTexture, CardAreaRight.x, CardAreaLeft.y + CardSlotTexture.height, WHITE);
        // TODO(nick): clean up this positioning
        Vector2 testPosition = 
        {
            .x = TableAreaCenter.x - (MrFrecklesSpritesheets[0].width / 4),
            .y = TableAreaCenter.y - ((MrFrecklesSpritesheets[0].height / 4) * 1.60f),
        };
        DrawAnimationFrame(&MrFrecklesSpritesheets[0], &MrFrecklesSpriteAnimation[0], &testPosition, GlobalTargetFPS);
    }

    EndDrawing();
    GlobalFrameCount++;
}

void
HandleConfirmButtonPress(Poker_Game* game_state)
{
    switch (game_state->poker_state)
    {
        case PokerState_NotStarted: {
            Poker_StartNewRound(game_state);
            game_state->poker_state = PokerState_Shuffled;
        }
        break;

        case PokerState_Shuffled: {
            for(int i = 0; i < 2; ++i)
            {
                // TODO: Card dealing / flipping animation
                game_state->player_hand[i] = Poker_DrawOne(CardState_Shown);
                game_state->dealer_hand[i] = Poker_DrawOne(CardState_Shown);
            }

            game_state->poker_state = PokerState_PlayerCardsDealt;
        }
        break;

        case PokerState_PlayerCardsDealt: {
            for (int i = 0; i < 3; ++i)
            {
                game_state->house_hand[i] = Poker_DrawOne(CardState_Shown);
            }

            game_state->poker_state = PokerState_FlopCardsDealt;

        }
        break;

        case PokerState_FlopCardsDealt: {
            game_state->house_hand[3] = Poker_DrawOne(CardState_Shown);

            game_state->poker_state = PokerState_RiverCardsDealt;
        }
        break;

        case PokerState_RiverCardsDealt: {
            game_state->house_hand[4] = Poker_DrawOne(CardState_Shown);

            game_state->poker_state = PokerState_TurnCardsDealt;
        }
        break;

        case PokerState_TurnCardsDealt: {
            // TODO: Award / Deduct points.
            // TODO: Lose / Win animations and sounds.

            game_state->poker_state = PokerState_GameOver;
        }
        break;

        case PokerState_GameOver: {
            // TODO: Continue / New Game / Quit

            game_state->poker_state = PokerState_NotStarted;
        }
        break;

        default:
            break;
    }

    Poker_ProcessNewState(game_state);
}

void
DrawHorizontalCardArea(Texture2D texture, Vector2 area, int card_count, float x_shift)
{
    for (int i = 0; i < card_count; ++i) 
    {
        DrawTexture(texture, area.x + (i * x_shift), area.y, WHITE);
    }
}

void
DrawFaceCard(Poker_Card card, int x, int y)
{
    int cardIndex = 0;
    switch (card.suit)
    {
        case CardSuit_Heart:
        {
            cardIndex = 0;
        } break;

        case CardSuit_Club:
        {
            cardIndex = 13;
        } break;

        case CardSuit_Diamond:
        {
            cardIndex = 26;
        } break;

        case CardSuit_Spade:
        {
            cardIndex = 39;
        } break;
    }
    if (card.face_value > CardFace_Two)
    {
        cardIndex += (card.face_value - 2);
    }
    if (card.face_value > CardFace_Ten && card.face_value != CardFace_Ace)
    {
        // TODO(nick): if it is a high face card then draw animation.
        Vector2 position = 
        {
            .x = x,
            .y = y,
        };
        DrawAnimationFrame(&CardTextures[cardIndex], &HighCardSpriteAnimation, &position, GlobalTargetFPS);
    }
    else
    {
        DrawTexture(CardTextures[cardIndex], x, y, WHITE);
    }
}
