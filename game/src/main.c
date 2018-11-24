/*
 *  main.c
 *
 */

#include <FC/game-screen.h>
#include <FC/sprite-animation.h>
#include <FC/poker.h>
#include <FC/defines.h>
#include <FC/scene.h>
#include <FC/character-state.h>
#include <FC/sound.h>
#include <FC/input.h>

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
#define GAME_WIDTH 320
#define GAME_HEIGHT 240

global_variable int GlobalWindowWidth = WIDTH;
global_variable int GlobalWindowHeight = HEIGHT;
global_variable const char *GlobalWindowTitle = "Mr. FC Casino";
global_variable unsigned char GlobalRunning = 1;
global_variable int GlobalFrameCount = 0;
global_variable int GlobalTargetFPS = 60;

global_variable Texture2D BlankGreenTableTexture;
global_variable Texture2D RedCurtainTextureHalf;
global_variable Texture2D RedCurtainTextureFull;
global_variable Texture2D CardSlotTexture;
global_variable Texture2D BackOfCardTexture;
global_variable Texture2D ScoreFrameTexture;

global_variable Texture2D TitleScreenLogo;
global_variable Vector2 TitleScreenLogoPosition;

global_variable Texture2D TitleScreenPressStart;
global_variable Vector2 TitleScreenPressStartPosition;

global_variable Texture2D CardTextures[CardSuit_Count * CardFace_Count];
global_variable SpriteAnimation HighCardSpriteAnimation;

global_variable Vector2 RedCurtainVector2;
global_variable Vector2 CardAreaLeft;
global_variable Vector2 CardAreaRight;
global_variable Vector2 CardAreaCenter;
global_variable Vector2 TableVector2;
global_variable Vector2 TableAreaCenter;
global_variable Vector2 ChancesBillboardArea;

global_variable unsigned int CurrentCharacterId = MrFreckles;

global_variable Texture2D MrFrecklesSpritesheets[4];
global_variable SpriteAnimation MrFrecklesSpriteAnimation[4];
global_variable Vector2 MrFrecklesPosition[4];
global_variable unsigned int MrFrecklesActiveState = Idle;

global_variable bool GameStarted = false;
global_variable unsigned int GameScene = SceneTitleScreen;

global_variable Music CharacterThemeMusic[5];
global_variable SoundMeta CharacterThemeMusicMeta[5];

global_variable Music MrFrecklesDialogue[26];
global_variable SoundMeta MrFrecklesDialogueMeta[26];

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
Render();

void
DrawHorizontalCardArea(Texture2D texture, Vector2 area, int card_count, float x_shift);

void
DrawFaceCard(Poker_Card card, int x, int y);

void
HandleConfirmButtonPress(Poker_Game* game_state);

void
RenderScene(Poker_Game* game_state, unsigned int scene);

void
RenderTitleScreen();

void
RenderGame(Poker_Game* game_state);

void
DrawFaceCard(Poker_Card card, int x, int y);

void
LoadSounds();

void
UnloadSounds();

void
InitSounds();

void
ExitGame();

int
main(void)
{
    local_persist Poker_Game game_state;
    // TODO(nick): replace with init game function
    GameScreen_Init(GlobalWindowWidth, GlobalWindowHeight, GAME_WIDTH, GAME_HEIGHT);
    Poker_Init(&game_state);
    InitWindow(GlobalWindowWidth, GlobalWindowHeight, GlobalWindowTitle);
    SetTargetFPS(GlobalTargetFPS);

    // NOTE: load all textures
    LoadTextures();

    // NOTE: load all sounds
    InitAudioDevice();
    LoadSounds();
    InitSounds();

    if (IsWindowReady())
    {
        PlaySounds();
        while (GlobalRunning)
        {
            UpdateSounds();
            ProcessInput(&game_state);
            RenderScene(&game_state, GameScene);
            if (WindowShouldClose())
            {
                GlobalRunning = 0;
            }
        }
    }

    ExitGame();

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
LoadTableAndBackgroundTextures(Image *tempImage, Vector2 *imageVector)
{
    // NOTE: load blank table green texture
    // @Size:
    *tempImage = LoadImage("assets/textures/Background/BlankTableGreen.png");
    ImageResizeNN(tempImage, GlobalWindowWidth, GlobalWindowHeight / 2.0f);
    BlankGreenTableTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load red curtain texture - half it
    // @Size: 
    *tempImage = LoadImage("assets/textures/Background/Curtain.png");
    ImageResizeNN(tempImage, GlobalWindowWidth, GlobalWindowHeight / 2.0f);
    RedCurtainTextureHalf = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load red curtain texture - full
    // @Size: 
    *tempImage = LoadImage("assets/textures/Background/Curtain.png");
    ImageResizeNN(tempImage, GlobalWindowWidth, GlobalWindowHeight);
    RedCurtainTextureFull = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load card slot texture
    // @Size: 15x20
    *tempImage = LoadImage("assets/textures/Background/CardSlot.png");
    imageVector->x = (tempImage->width * 2.5f);
    imageVector->y = (tempImage->height * 2.5f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    CardSlotTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load brass plate texture
    *tempImage = LoadImage("assets/textures/Background/BrassPlate.png");
    imageVector->x = (CardSlotTexture.width * 2.0f) + GameScreen_LocalUnitsToScreen(1.0f);
    imageVector->y = CardSlotTexture.height;
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    ScoreFrameTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
}

inline void
LoadCardsTextures(Texture2D CardTextures[52], Texture2D *BackOfCardTexture)
{
    // Back of Card
    // @Size: 15x12
    LoadCardTexture("assets/textures/Cards/BackOfCard/BackOfCard.png", BackOfCardTexture);

    // Hearts
    // @Size: 15x12
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/2Hearts.png", &CardTextures[0]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/3Hearts.png", &CardTextures[1]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/4Hearts.png", &CardTextures[2]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/5Hearts.png", &CardTextures[3]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/6Hearts.png", &CardTextures[4]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/7Hearts.png", &CardTextures[5]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/8Hearts.png", &CardTextures[6]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/9Hearts.png", &CardTextures[7]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/10Hearts.png", &CardTextures[8]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/JackHearts.png", &CardTextures[9]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/QueenHearts.png", &CardTextures[10]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/KingHearts.png", &CardTextures[11]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/AceHearts.png", &CardTextures[12]);

    // Clubs
    // @Size: 15x12
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/2Clubs.png", &CardTextures[13]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/3Clubs.png", &CardTextures[14]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/4Clubs.png", &CardTextures[15]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/5Clubs.png", &CardTextures[16]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/6Clubs.png", &CardTextures[17]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/7Clubs.png", &CardTextures[18]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/8Clubs.png", &CardTextures[19]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/9Clubs.png", &CardTextures[20]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/10Clubs.png", &CardTextures[21]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/JackClubs.png", &CardTextures[22]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/QueenClubs.png", &CardTextures[23]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/KingClubs.png", &CardTextures[24]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/AceClubs.png", &CardTextures[25]);

    // Diamonds
    // @Size: 15x12
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/2Dia.png", &CardTextures[26]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/3Dia.png", &CardTextures[27]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/4Dia.png", &CardTextures[28]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/5Dia.png", &CardTextures[29]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/6Dia.png", &CardTextures[30]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/7Dia.png", &CardTextures[31]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/8Dia.png", &CardTextures[32]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/9Dia.png", &CardTextures[33]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/10Dia.png", &CardTextures[34]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/JackDia.png", &CardTextures[35]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/QueenDia.png", &CardTextures[36]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/KingDia.png", &CardTextures[37]);
    LoadCardTexture("assets/textures/Cards/Diamonds/Pngs/AceDia.png", &CardTextures[38]);

    // Spades
    // @Size: 15x12
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/2Spades.png", &CardTextures[39]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/3Spades.png", &CardTextures[40]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/4Spades.png", &CardTextures[41]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/5Spades.png", &CardTextures[42]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/6Spades.png", &CardTextures[43]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/7Spades.png", &CardTextures[44]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/8Spades.png", &CardTextures[45]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/9Spades.png", &CardTextures[46]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/10Spades.png", &CardTextures[47]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/JackSpades.png", &CardTextures[48]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/QueenSpades.png", &CardTextures[49]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/KingSpades.png", &CardTextures[50]);
    LoadCardTexture("assets/textures/Cards/Spades/Pngs/AceSpades.png", &CardTextures[51]);
}

inline
LoadCharacterTextures(Image *tempImage, Vector2 *imageVector)
{
    // NOTE: Mr. Freckles Spritesheets Begin
    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/Idle.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 3.0f);
    imageVector->x += GameScreen_LocalUnitsToScreen(2.0f);
    imageVector->y += GameScreen_LocalUnitsToScreen(2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[Idle] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[Idle] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[Idle].width, MrFrecklesSpritesheets[Idle].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/PlayingCards.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 3.0f);
    imageVector->x += GameScreen_LocalUnitsToScreen(2.0f);
    imageVector->y += GameScreen_LocalUnitsToScreen(2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[PlayingCards] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[PlayingCards] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[PlayingCards].width, MrFrecklesSpritesheets[PlayingCards].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/Winning.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 3.0f);
    imageVector->x += GameScreen_LocalUnitsToScreen(2.0f);
    imageVector->y += GameScreen_LocalUnitsToScreen(2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[Winning] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[Winning] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[Winning].width, MrFrecklesSpritesheets[Winning].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/Losing.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 3.0f);
    imageVector->x += GameScreen_LocalUnitsToScreen(2.0f);
    imageVector->y += GameScreen_LocalUnitsToScreen(2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[Losing] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[Losing] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[Losing].width, MrFrecklesSpritesheets[Losing].height);
    // NOTE: Mr. Freckles Spritesheets End
}

inline
LoadTitleScreen(Image *tempImage, Vector2 *imageVector)
{
    *tempImage = LoadImage("assets/textures/Titlescreen/Pngs/TitleLogo.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    TitleScreenLogo = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    *tempImage = LoadImage("assets/textures/Titlescreen/Pngs/PressStart.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 3.0f);
    imageVector->x += GameScreen_LocalUnitsToScreen(1.0f);
    imageVector->y += GameScreen_LocalUnitsToScreen(1.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    TitleScreenPressStart = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
}

inline
SetPositions()
{
    TableVector2.y = (GlobalWindowHeight - BlankGreenTableTexture.height);
    TableVector2.x = 0;

    TableAreaCenter.y = TableVector2.y;
    TableAreaCenter.x = (BlankGreenTableTexture.width / 2.0f);

    CardAreaLeft.x = TableVector2.x + GameScreen_LocalUnitsToScreen(10.0f);
    CardAreaLeft.y = TableVector2.y + GameScreen_LocalUnitsToScreen(25.0f);

    RedCurtainVector2.x = 0;
    RedCurtainVector2.x = 0;

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

    TitleScreenLogoPosition.x = TableAreaCenter.x - (TitleScreenLogo.width / 2.0f);
    TitleScreenLogoPosition.y = 0;

    // TODO(nick): iron out these positions
    TitleScreenPressStartPosition.x = TableAreaCenter.x - (TitleScreenPressStart.width / 2.0f);
    TitleScreenPressStartPosition.y = TableAreaCenter.y + TitleScreenPressStart.height + GameScreen_LocalUnitsToScreen(35.0f);

    int xOffset = 0;
    int yOffset = 0;
    for (unsigned int currentState = Idle; currentState < len(MrFrecklesPosition); currentState++)
    {
        // NOTE: calculate one frame size and get one-half of one frame size
        if (MrFrecklesSpriteAnimation[currentState].totalFrames > 0) 
        {
            xOffset = (MrFrecklesSpritesheets[currentState].width / MrFrecklesSpriteAnimation[currentState].totalHorizontalFrames) * 0.5f;
            yOffset = (MrFrecklesSpritesheets[currentState].height / MrFrecklesSpriteAnimation[currentState].totalVerticalFrames) - GameScreen_LocalUnitsToScreen(35.0f);
            MrFrecklesPosition[currentState].x = TableAreaCenter.x - xOffset;
            MrFrecklesPosition[currentState].y = TableAreaCenter.y - yOffset;
        }
    }
}

void
LoadTextures()
{
    Image tempImage = { 0 };
    Vector2 imageVector = { 0 };

    // NOTE: load title screen textures
    LoadTitleScreen(&tempImage, &imageVector);

    // NOTE: load table and background textures
    LoadTableAndBackgroundTextures(&tempImage, &imageVector);

    // NOTE: load card textures
    LoadCardsTextures(CardTextures, &BackOfCardTexture);

    // NOTE: load character textures
    LoadCharacterTextures(&tempImage, &imageVector);

    // NOTE: set positions
    SetPositions();
}

void
UnloadTextures()
{
    UnloadTexture(BlankGreenTableTexture);
    UnloadTexture(CardSlotTexture);
    UnloadTexture(BackOfCardTexture);
    UnloadTexture(ScoreFrameTexture);
    int i = 0;
    for (i = 0; i < len(MrFrecklesSpritesheets); i++)
    {
        UnloadTexture(MrFrecklesSpritesheets[i]);
    }
}



void
RenderTitleScreen()
{
    BeginDrawing();
    {
        ClearBackground(BLACK);
        DrawTexture(RedCurtainTextureFull, RedCurtainVector2.x, RedCurtainVector2.y, WHITE);
        // TODO(nick): add slide in animation code.
        DrawTexture(TitleScreenLogo, TitleScreenLogoPosition.x, TitleScreenLogoPosition.y, WHITE);
        DrawTexture(TitleScreenPressStart, TitleScreenPressStartPosition.x, TitleScreenPressStartPosition.y, WHITE);
    }
    EndDrawing();
}

void
RenderGame(Poker_Game* game_state)
{
    BeginDrawing();
    {
        ClearBackground(BLACK);
        DrawTexture(RedCurtainTextureHalf, RedCurtainVector2.x, RedCurtainVector2.y, WHITE);
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
    
        Texture2D *currentCharacterSpritesheet = NULL;
        SpriteAnimation *currentCharacterAnimation = NULL;
        Vector2 *currentCharacterSpritePosition = NULL;

        switch (CurrentCharacterId)
        {
            case MrFreckles:
            {
                currentCharacterSpritesheet = &MrFrecklesSpritesheets[MrFrecklesActiveState];
                currentCharacterAnimation = &MrFrecklesSpriteAnimation[MrFrecklesActiveState];
                currentCharacterSpritePosition = &MrFrecklesPosition[MrFrecklesActiveState];
            } break;
        }
        DrawAnimationFrame(currentCharacterSpritesheet, currentCharacterAnimation, currentCharacterSpritePosition, GlobalTargetFPS);
    }

    EndDrawing();
    GlobalFrameCount++;
}

void
RenderScene(Poker_Game* game_state, unsigned int scene)
{
    switch (scene)
    {
        case SceneMainPokerTable:
        {
            RenderGame(game_state);
        } break;

        default:
        {
            RenderTitleScreen();
        } break;
    }
}

void
HandleConfirmButtonPress(Poker_Game* game_state)
{
    switch (game_state->poker_state)
    {
        case PokerState_NotStarted: 
        {
            Poker_StartNewRound(game_state);
            game_state->poker_state = PokerState_Shuffled;
        } break;

        case PokerState_Shuffled: 
        {
            for(int i = 0; i < 2; ++i)
            {
                // TODO: Card dealing / flipping animation
                game_state->player_hand[i] = Poker_DrawOne(CardState_Shown);
                game_state->dealer_hand[i] = Poker_DrawOne(CardState_Shown);
            }

            game_state->poker_state = PokerState_PlayerCardsDealt;
        } break;

        case PokerState_PlayerCardsDealt: {
            for (int i = 0; i < 3; ++i)
            {
                game_state->house_hand[i] = Poker_DrawOne(CardState_Shown);
            }

            game_state->poker_state = PokerState_FlopCardsDealt;

        } break;

        case PokerState_FlopCardsDealt: 
        {
            game_state->house_hand[3] = Poker_DrawOne(CardState_Shown);

            game_state->poker_state = PokerState_RiverCardsDealt;
        } break;

        case PokerState_RiverCardsDealt:
        {
            game_state->house_hand[4] = Poker_DrawOne(CardState_Shown);

            game_state->poker_state = PokerState_TurnCardsDealt;
        } break;

        case PokerState_TurnCardsDealt:
        {
            // TODO: Award / Deduct points.
            // TODO: Lose / Win animations and sounds.
            game_state->poker_state = PokerState_GameOver;
        } break;

        case PokerState_GameOver:
        {
            // TODO: Continue / New Game / Quit
            game_state->poker_state = PokerState_NotStarted;
        } break;

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
    DrawTexture(CardTextures[cardIndex], x, y, WHITE);
}

void
LoadSounds()
{
    // NOTE: load theme music
    CharacterThemeMusic[MrFreckles] = LoadMusicStream("assets/sounds/music/ogg/Mr_Freckles.ogg");
    CharacterThemeMusic[MrsFreckles] = LoadMusicStream("assets/sounds/music/ogg/Mrs_Freckles.ogg");
    CharacterThemeMusic[ColHindenburger] = LoadMusicStream("assets/sounds/music/ogg/Col_Von_HindenBurger.ogg");
    CharacterThemeMusic[GeneralGrunt] = LoadMusicStream("assets/sounds/music/ogg/Generalissimo_Grunt.ogg");
    CharacterThemeMusic[Pyrella] = LoadMusicStream("assets/sounds/music/ogg/Pyrella.ogg");

    InitializeSoundMeta(CharacterThemeMusic, CharacterThemeMusicMeta, len(CharacterThemeMusic));

    // NOTE: load Mr. Freckles dialogue
    MrFrecklesDialogue[0] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Laugh/Laugh1.ogg");
    MrFrecklesDialogue[1] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Laugh/Laugh2.ogg");
    MrFrecklesDialogue[2] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_1.ogg");
    MrFrecklesDialogue[3] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_2.ogg");
    MrFrecklesDialogue[4] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_3.ogg");
    MrFrecklesDialogue[5] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_4.ogg");
    MrFrecklesDialogue[6] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_5.ogg");
    MrFrecklesDialogue[7] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_6.ogg");
    MrFrecklesDialogue[8] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_7.ogg");
    MrFrecklesDialogue[9] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_8.ogg");
    MrFrecklesDialogue[10] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_9.ogg");
    MrFrecklesDialogue[11] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_10.ogg");
    MrFrecklesDialogue[12] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Lose/FrecklesLose0_11.ogg");
    MrFrecklesDialogue[13] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Start/FrecklesStart0_1.ogg");
    MrFrecklesDialogue[14] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Start/FrecklesStart0_2.ogg");
    MrFrecklesDialogue[15] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Start/FrecklesStart0_3.ogg");
    MrFrecklesDialogue[16] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Start/FrecklesStart0_4.ogg");
    MrFrecklesDialogue[17] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Start/FrecklesStart0_5.ogg");
    MrFrecklesDialogue[18] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Start/FrecklesStart0_6.ogg");
    MrFrecklesDialogue[19] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Start/FrecklesStart0_7.ogg");
    MrFrecklesDialogue[20] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Win/FrecklesWin0_1.ogg");
    MrFrecklesDialogue[21] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Win/FrecklesWin0_2.ogg");
    MrFrecklesDialogue[22] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Win/FrecklesWin0_3.ogg");
    MrFrecklesDialogue[23] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Win/FrecklesWin0_4.ogg");
    MrFrecklesDialogue[24] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Win/FrecklesWin0_5.ogg");
    MrFrecklesDialogue[25] = LoadMusicStream("assets/sounds/dialogue/MrFreckles/Win/FrecklesWin0_6.ogg");
    
    InitializeSoundMeta(MrFrecklesDialogue, MrFrecklesDialogueMeta, len(MrFrecklesDialogue));
}

void
UnloadSounds()
{
    int i;
    for (i = 0; i < len(CharacterThemeMusic); i++)
    {
        UnloadMusicStream(CharacterThemeMusic[i]);
    }
    
    for (i = 0; i < len(MrFrecklesDialogue); i++)
    {
        UnloadMusicStream(MrFrecklesDialogue[i]);
    }
}

void
InitSounds()
{
    CharacterThemeMusicMeta[MrFreckles].playLimit = INFINITE_PLAY;
    AddSoundToBuffer(&CharacterThemeMusic[MrFreckles], &CharacterThemeMusicMeta[MrFreckles]);

    MrFrecklesDialogueMeta[18].playLimit = 1;
    AddSoundToBuffer(&MrFrecklesDialogue[18], &MrFrecklesDialogueMeta[18]);
}

void
ExitGame()
{
    UnloadTextures();
    UnloadSounds();
    CloseAudioDevice();
    CloseWindow();
}
