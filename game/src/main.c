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

global_variable Vector2 CenterScreenPosition;

// Title Screen Texture(s) / Animation(s) / Position(s)
global_variable Texture2D TitleScreenSpriteSheet;
global_variable SpriteAnimation TitleScreenSpriteAnimation;
global_variable Vector2 TitleScreenAnimationPosition;

// Background Texture(s) / Animation(s) / Position(s)
global_variable Texture2D BorderTexture;
global_variable Vector2 BorderPosition;
global_variable Texture2D RedCurtainTexture;
global_variable Vector2 RedCurtainPosition;
global_variable Texture2D GreenTableTexture;
global_variable Vector2 GreenTablePosition;
global_variable Texture2D CardSlotTexture;
global_variable Vector2 CardSlotPositions[10];

// Misc Texture(s) / Position(s)
global_variable Texture2D FrecklesNamePlateTexture;
global_variable Vector2 FrecklesNamePlatePosition;
global_variable Texture2D NamePlateTexture;
global_variable Vector2 NamePlatePosition;
global_variable Texture2D BankTexture;
global_variable Vector2 BankPosition;
global_variable Texture2D HoldCursorTexture;
global_variable Vector2 HoldCursorPositions[5];
global_variable unsigned char DrawHoldCursor[5];
global_variable unsigned char CurrentHoldCursorIndex;
global_variable Texture2D SpeechBubbleTexture;
global_variable Vector2 SpeechBubblePosition;

// Card Texture(s) / Position(s)
global_variable Texture2D BackOfCardTexture;
global_variable Texture2D CardTextures[CardSuit_Count * CardFace_Count];
global_variable Vector2 CardPositions[10];

// Character Texture(s) / Animation(s) / Position(s)
global_variable Texture2D MrFrecklesSpritesheets[13];
global_variable SpriteAnimation MrFrecklesSpriteAnimation[13];
global_variable Vector2 MrFrecklesPosition[13];

global_variable unsigned int MrFrecklesActiveState = Idle;
global_variable unsigned int CurrentCharacterId = MrFreckles;

global_variable bool GameStarted = false;

global_variable Music CharacterThemeMusic[5];
global_variable SoundMeta CharacterThemeMusicMeta[5];

global_variable Music MrFrecklesDialogue[26];
global_variable SoundMeta MrFrecklesDialogueMeta[26];

const char* CreditsText = "CREDITS";
const char* JackpotText = "JACKPOT";
const char* BetText     = "BET";

inline void 
GameInit(Poker_Game *game_state, Game_Scene_State *game_scene_state, Game_Input_State *game_input_state);

void
LoadTextures();

void
LoadCardTexture(char *filePath, Texture2D *texture);

void
LoadCardsTextures(Texture2D CardTextures[52], Texture2D *BackOfCardTexture);

void
LoadCharacterTextures(Image *tempImage, Vector2 *imageVector);

void
LoadTitleScreen(Image *tempImage, Vector2 *imageVector);

void
LoadTableAndBackgroundTextures(Image *tempImage, Vector2 *imageVector);

void
SetPositions();

void
UnloadTextures();

void
Render();

void
DrawHorizontalCardArea(Texture2D texture, Vector2 area, int card_count, float x_shift);

void
DrawFaceCard(Poker_Card card, int x, int y);

void
RenderScene(Poker_Game* game_state, Game_Input_State *game_input_state, unsigned int scene);

void
RenderTitleScreen();

void
RenderGame(Poker_Game* game_state, Game_Input_State *game_input_state);

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
    local_persist Game_Scene_State game_scene_state;
    local_persist Game_Input_State game_input_state;
    GameInit(&game_state, &game_scene_state, &game_input_state);
    if (IsWindowReady())
    {
        PlaySounds();
        while (GlobalRunning)
        {
            UpdateSounds();
            ProcessInput(&game_state, &game_scene_state, &game_input_state);
            RenderScene(&game_state, &game_input_state, game_scene_state.current_scene);
            if (WindowShouldClose())
            {
                GlobalRunning = 0;
            }
        }
    }
    ExitGame();
    return 0;
}

inline void
GameInit(Poker_Game *game_state, Game_Scene_State *game_scene_state, Game_Input_State *game_input_state)
{
    zero_array(DrawHoldCursor);
    Init_Input_State(game_input_state);
    GameScreen_Init(GlobalWindowWidth, GlobalWindowHeight, GAME_WIDTH, GAME_HEIGHT);
    // TODO(nick): create a main menu that allows user to pick game type
    Poker_Init(game_state, GameType_FiveCard);
    *game_scene_state = Init_Game_Scene_State();
    InitWindow(GlobalWindowWidth, GlobalWindowHeight, GlobalWindowTitle);
    SetTargetFPS(GlobalTargetFPS);
    // NOTE: load all textures
    LoadTextures();
    // NOTE: load all sounds
    InitAudioDevice();
    LoadSounds();
    InitSounds();
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
    // NOTE: load border texture
    *tempImage = LoadImage("assets/textures/Background/border.png");
    ImageResizeNN(tempImage, GlobalWindowWidth, GlobalWindowHeight);
    BorderTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load red curtain texture
    *tempImage = LoadImage("assets/textures/Background/red-curtain.png");
    ImageResizeNN(tempImage, GlobalWindowWidth, GlobalWindowHeight * 0.6f);
    RedCurtainTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load green table texture
    *tempImage = LoadImage("assets/textures/Background/green-table.png");
    ImageResizeNN(tempImage, GlobalWindowWidth, GlobalWindowHeight * 0.4f);
    GreenTableTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load card slot texture
    *tempImage = LoadImage("assets/textures/Misc/card-slot-texture.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    CardSlotTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load name plate texture
    *tempImage = LoadImage("assets/textures/Misc/name-plate.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    NamePlateTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load score frame texture
    *tempImage = LoadImage("assets/textures/Misc/bank.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    BankTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load hold cursor texture
    *tempImage = LoadImage("assets/textures/Misc/hold-cursor.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    HoldCursorTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);

    // NOTE: load speech bubble texture
    *tempImage = LoadImage("assets/textures/Misc/speech-bubble.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    SpeechBubbleTexture = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
}

inline void
LoadCardsTextures(Texture2D CardTextures[52], Texture2D *BackOfCardTexture)
{
    // Back of Card
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
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/JackHearts.png", &CardTextures[9]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/QueenHearts.png", &CardTextures[10]);
    LoadCardTexture("assets/textures/Cards/Hearts/Pngs/KingHearts.png", &CardTextures[11]);
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
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/JackClubs.png", &CardTextures[22]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/QueenClubs.png", &CardTextures[23]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/KingClubs.png", &CardTextures[24]);
    LoadCardTexture("assets/textures/Cards/Clubs/Pngs/AceClubs.png", &CardTextures[25]);

    // Diamonds
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

inline void
LoadCharacterTextures(Image *tempImage, Vector2 *imageVector)
{
    // NOTE: Mr. Freckles Spritesheets Begin
    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-idle.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[Idle] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[Idle] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[Idle].width, MrFrecklesSpritesheets[Idle].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-gameplay.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[GamePlay] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[GamePlay] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[GamePlay].width, MrFrecklesSpritesheets[GamePlay].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-gameplay-level-final-1.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[GamePlayFinal_1] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[GamePlayFinal_1] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[GamePlayFinal_1].width, MrFrecklesSpritesheets[GamePlayFinal_1].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-gameplay-level-final-2.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[GamePlayFinal_2] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[GamePlayFinal_2] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[GamePlayFinal_2].width, MrFrecklesSpritesheets[GamePlayFinal_2].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-idle-stress-level-1.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[IdleStress_1] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[IdleStress_1] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[IdleStress_1].width, MrFrecklesSpritesheets[IdleStress_1].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-idle-stress-level-2.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[IdleStress_2] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[IdleStress_2] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[IdleStress_2].width, MrFrecklesSpritesheets[IdleStress_2].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-idle-stress-level-3.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[IdleStress_3] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[IdleStress_3] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[IdleStress_3].width, MrFrecklesSpritesheets[IdleStress_3].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-talk-stress-level-1.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[TalkStress_1] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[TalkStress_1] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[TalkStress_1].width, MrFrecklesSpritesheets[TalkStress_1].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-talk-stress-level-2.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[TalkStress_2] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[TalkStress_2] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[TalkStress_2].width, MrFrecklesSpritesheets[TalkStress_2].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-talk-stress-level-3.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[TalkStress_3] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[TalkStress_3] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[TalkStress_3].width, MrFrecklesSpritesheets[TalkStress_3].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-talk-stress-level-final.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[TalkStressFinal] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[TalkStressFinal] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[TalkStressFinal].width, MrFrecklesSpritesheets[TalkStressFinal].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-lose.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[Lose] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[Lose] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[Lose].width, MrFrecklesSpritesheets[Lose].height);

    *tempImage = LoadImage("assets/textures/Characters/Spritesheets/MrFreckles/freckles-win.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    MrFrecklesSpritesheets[Win] = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    MrFrecklesSpriteAnimation[Win] = CreateSpriteAnimation(2, 1, 2, 2, MrFrecklesSpritesheets[Win].width, MrFrecklesSpritesheets[Win].height);
    // NOTE: Mr. Freckles Spritesheets End
}

inline void
LoadTitleScreen(Image *tempImage, Vector2 *imageVector)
{
    *tempImage = LoadImage("assets/textures/Titlescreen/title-screen-spritesheet.png");
    imageVector->x = tempImage->width;
    imageVector->y = tempImage->height;
    *imageVector = Vector2Scale(*imageVector, 2.0f);
    *imageVector = Vector2Scale(*imageVector, GameScreen_ScreenUnitScale());
    ImageResizeNN(tempImage, imageVector->x, imageVector->y);
    TitleScreenSpriteSheet = LoadTextureFromImage(*tempImage);
    UnloadImage(*tempImage);
    TitleScreenSpriteAnimation = CreateSpriteAnimation(28, 1, 28, 10, TitleScreenSpriteSheet.width, TitleScreenSpriteSheet.height);
}

inline void
SetPositions()
{
    // Set center screen position
    CenterScreenPosition.x = (GlobalWindowWidth / 2.0f);
    CenterScreenPosition.y = (GlobalWindowHeight / 2.0f);

    // Set all title screen position(s)
    TitleScreenAnimationPosition.x = CenterScreenPosition.x - (TitleScreenSpriteSheet.width / TitleScreenSpriteAnimation.totalHorizontalFrames) * 0.5f;
    TitleScreenAnimationPosition.y = CenterScreenPosition.y - (TitleScreenSpriteSheet.height / TitleScreenSpriteAnimation.totalVerticalFrames) * 0.5f;
    // Set all game background positions
    BorderPosition.x = 0;
    BorderPosition.y = 0;
    RedCurtainPosition.x = 0;
    RedCurtainPosition.y = 0;
    GreenTablePosition.x = RedCurtainPosition.x;
    GreenTablePosition.y = RedCurtainPosition.y + RedCurtainTexture.height;
    NamePlatePosition.x = (CenterScreenPosition.x - (NamePlateTexture.width * 0.5f));
    NamePlatePosition.y = (GreenTablePosition.y + GameScreen_LocalUnitsToScreen(10.0f));
    BankPosition.x = BorderPosition.x + GameScreen_LocalUnitsToScreen(20.0f);
    BankPosition.y = GreenTablePosition.y - BankTexture.height - GameScreen_LocalUnitsToScreen(20.0f);
    Vector2 CardSlotStartingPositionTop =
    {
        .x = CenterScreenPosition.x - (CardSlotTexture.width * 0.5f) - (CardSlotTexture.width * 3.0f),
        .y = CenterScreenPosition.y - (CardSlotTexture.height * 2.5f),
    };
    Vector2 CardSlotStartingPositionBottom = 
    {
        .x = CardSlotStartingPositionTop.x,
        .y = CenterScreenPosition.y + (CardSlotTexture.height * 1.5f),
    };
    float CardSlotPadding = GameScreen_LocalUnitsToScreen(16.0f);
    int offset = 5;
    for (unsigned int i = 0; i < len(CardSlotPositions) / 2; i++)
    {
        CardSlotPositions[i].x = CardSlotStartingPositionTop.x + (CardSlotPadding * i) + (CardSlotTexture.width * i);
        CardSlotPositions[i].y = CardSlotStartingPositionTop.y;
        CardSlotPositions[i + offset].x = CardSlotStartingPositionBottom.x + (CardSlotPadding * i) + (CardSlotTexture.width * i);
        CardSlotPositions[i + offset].y = CardSlotStartingPositionBottom.y;
    }
    for (unsigned int i = 0; i < len(CardPositions) / 2; i++)
    {
        CardPositions[i].x = CardSlotStartingPositionTop.x + (CardSlotPadding * i) + (CardSlotTexture.width * i);
        CardPositions[i].y = CardSlotStartingPositionTop.y + GameScreen_LocalUnitsToScreen(2.0f);
        CardPositions[i + offset].x = CardSlotStartingPositionTop.x + (CardSlotPadding * i) + (CardSlotTexture.width * i);
        CardPositions[i + offset].y = CardSlotStartingPositionBottom.y + GameScreen_LocalUnitsToScreen(2.0f);
        HoldCursorPositions[i].x = CardPositions[i + offset].x;
        HoldCursorPositions[i].y = CardPositions[i + offset].y + HoldCursorTexture.width + (CardSlotTexture.width * 0.5f);
    }
    // Set the characters position(s)
    int xOffset = 0;
    int yOffset = 0;
    for (unsigned int currentState = Idle; currentState < len(MrFrecklesPosition); currentState++)
    {
        // NOTE: calculate one frame size and get one-half of one frame size
        if (MrFrecklesSpriteAnimation[currentState].totalFrames > 0) 
        {
            xOffset = (MrFrecklesSpritesheets[currentState].width / MrFrecklesSpriteAnimation[currentState].totalHorizontalFrames) * 0.5f;
            yOffset = (MrFrecklesSpritesheets[currentState].height / MrFrecklesSpriteAnimation[currentState].totalVerticalFrames);
            MrFrecklesPosition[currentState].x = CenterScreenPosition.x - xOffset;
            MrFrecklesPosition[currentState].y = GreenTablePosition.y - yOffset;
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

// TODO(nick): update this function to unload all loaded in texture ...
void
UnloadTextures()
{
    UnloadTexture(BackOfCardTexture);
    int i = 0;
    for (i = 0; i < len(MrFrecklesSpritesheets); i++)
    {
        UnloadTexture(MrFrecklesSpritesheets[i]);
    }
}

void
RenderTitleScreen()
{
    // TODO(nick):
    // 1) add slide in animation code.
    // 2) static draw after first animation loop - then start again at a particular time set interval
    BeginDrawing();
    {
        ClearBackground(BLACK);
        DrawAnimationFrame(&TitleScreenSpriteSheet, &TitleScreenSpriteAnimation, &TitleScreenAnimationPosition, GlobalTargetFPS);
    }
    EndDrawing();
}

void
RenderGame(Poker_Game* game_state, Game_Input_State *game_input_state)
{
    BeginDrawing();
    {
        ClearBackground(BLACK);
        DrawTexture(RedCurtainTexture, RedCurtainPosition.x, RedCurtainPosition.y, WHITE);
        DrawTexture(GreenTableTexture, GreenTablePosition.x, GreenTablePosition.y, WHITE);
        DrawTexture(NamePlateTexture, NamePlatePosition.x, NamePlatePosition.y, WHITE);
        DrawTexture(BankTexture, BankPosition.x, BankPosition.y, WHITE);
        DrawTexture(BorderTexture, BorderPosition.x, BorderPosition.y, WHITE);
        for (unsigned int i = 0; i < len(CardSlotPositions); i++)
        {
            DrawTexture(CardSlotTexture, CardSlotPositions[i].x, CardSlotPositions[i].y, WHITE);
        }
        if (CurrentHoldCursorIndex >= 0) 
        {
            DrawTexture(HoldCursorTexture, HoldCursorPositions[game_input_state->hold_cursor_index].x, HoldCursorPositions[game_input_state->hold_cursor_index].y, WHITE);
            /*
            for (unsigned int i = 0; i < len(HoldCursorPositions); i++)
            {
                DrawTexture(HoldCursorTexture, HoldCursorPositions[i].x, HoldCursorPositions[i].y, WHITE);
            }
            */
        }
        unsigned int offset = 5;
        for (unsigned int i = 0; i < len(CardPositions) / 2; i++)
        {
            // TODO(nick): game state needs to change as poker rules have changed 
            // no longer texas hold'em, just straight 5 card
            if (game_state->dealer_hand[i].state == CardState_Hidden)
            {
                DrawTexture(BackOfCardTexture, CardPositions[i].x, CardPositions[i].y, WHITE);
            }
            else if (game_state->dealer_hand[i].state == CardState_Shown)
            {
                DrawFaceCard(game_state->dealer_hand[i], CardPositions[i].x, CardPositions[i].y);
            }

            if (game_state->player_hand[i].state == CardState_Hidden)
            {
                DrawTexture(BackOfCardTexture, CardPositions[i + offset].x, CardPositions[i + offset].y, WHITE);
            }
            else if (game_state->player_hand[i].state == CardState_Shown)
            {
                DrawFaceCard(game_state->player_hand[i], CardPositions[i + offset].x, CardPositions[i + offset].y);
            }
        }
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
RenderScene(Poker_Game *game_state, Game_Input_State *game_input_state, Scene scene)
{
    switch (scene)
    {
        case Scene_MainPokerTable:
        {
            RenderGame(game_state, game_input_state);
        } break;

        default:
        {
            RenderTitleScreen();
        } break;
    }
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
        if (CharacterThemeMusic[i] != NULL)
        {
            UnloadMusicStream(CharacterThemeMusic[i]);
        }
    }
    
    for (i = 0; i < len(MrFrecklesDialogue); i++)
    {
        if (MrFrecklesDialogue[i] != NULL)
        {
            UnloadMusicStream(MrFrecklesDialogue[i]);
        }
    }
}

void
InitSounds()
{
    CharacterThemeMusicMeta[MrFreckles].playLimit = INFINITE_PLAY;
    AddSoundToBuffer(&CharacterThemeMusic[MrFreckles], &CharacterThemeMusicMeta[MrFreckles]);
    //MrFrecklesDialogueMeta[18].playLimit = 1;
    //AddSoundToBuffer(&MrFrecklesDialogue[18], &MrFrecklesDialogueMeta[18]);
}

void
ExitGame()
{
    UnloadTextures();
    UnloadSounds();
    CloseAudioDevice();
    CloseWindow();
}
