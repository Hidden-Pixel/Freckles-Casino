/*
 *  sprite-animation.h
 *
 */

#include "raylib.h"

// TODO(nick): place in another file?
typedef enum _assetType
{
    AssetType_Texture2D     = 0x00,
    AssetType_Text          = 0x01,
} AssetType;

typedef struct _spriteAnimation 
{
    unsigned int    totalFrames;
    unsigned int    totalVerticalFrames;
    unsigned int    totalHorizontalFrames;
    unsigned int    totalFramesRendered;
    float           frameWidth;
    float           frameHeight;
    unsigned int    currentDrawFrameIndex;
    unsigned int    frameCounter;
    unsigned int    frameSpeed;
    unsigned char   frameOrder;
    Rectangle       currentFrameRect;
    unsigned int    pauseFrame;
    unsigned int    pauseFrameDuration;
    bool            paused;
} SpriteAnimation;

typedef struct _blinkAnimation
{
    unsigned int    blinksPerSecond;
    unsigned int    blinkDurationFrames;
    unsigned int    unrenderedCounter;
    unsigned int    frameCounter;
} BlinkAnimation;

typedef struct _slideAnimation
{
    unsigned int    ySlidePerSecond;
    unsigned int    xSlidePerSecond;
    unsigned int    frameCounter;
} SlideAnimation;

typedef struct _fadeAnimation
{
    unsigned int    frameRenderDurationSeconds;
    unsigned int    fadeInDurationFrames;
    unsigned int    fadeOutDurationFrames;
    float           fadeCurrentAlphaValue;
    unsigned int    frameCounter;
} FadeAnimation;

SpriteAnimation
CreateSpriteAnimation(unsigned int totalFrames,
        unsigned int verticalFrames,
        unsigned int horizontalFrames,
        unsigned int frameSpeed,
        int sheetWidth, int sheetHeight);

SpriteAnimation
CreateSpriteAnimationWithPause(unsigned int totalFrames,
        unsigned int verticalFrames,
        unsigned int horizontalFrames,
        unsigned int frameSpeed,
        int sheetWidth, int sheetHeight,
        unsigned int pauseFrame,
        unsigned int pauseFrameDuration);

void
DrawAnimationFrame(Texture2D *spritesheet, SpriteAnimation *spriteAnimation, Vector2 *spritePosition, int gameFPS);

void
DrawBlinkAnimation(void *spritesheet, AssetType assetType, BlinkAnimation *blinkAnimation, Vector2 *spritePosition, int gameFPS);

BlinkAnimation
CreateBlinkAnimation(unsigned int blinksPerSecond);

FadeAnimation
CreateFadeAnimation(unsigned int frameRenderDurationSeconds, unsigned int fadeInDurationFrames, unsigned int fadeOutDurationFrames);

void
DrawFadeAnimation(void *asset, AssetType assetType, FadeAnimation *fade, Vector2 *position, int gameFPS);
