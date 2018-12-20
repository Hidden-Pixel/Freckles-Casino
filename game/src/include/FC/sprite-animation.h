/*
 *  sprite-animation.h
 *
 */

#include "raylib.h"

typedef struct _spriteAnimation 
{
    unsigned int    totalFrames;
    unsigned int    totalVerticalFrames;
    unsigned int    totalHorizontalFrames;
    float           frameWidth;
    float           frameHeight;
    unsigned int    currentDrawFrameIndex;
    unsigned int    frameCounter;
    unsigned int    frameSpeed;
    unsigned char   frameOrder;
    Rectangle       currentFrameRect;
} SpriteAnimation;

typedef struct _blinkAnimation
{
    unsigned int    blinksPerSecond;
    unsigned int    blinkDurationFrames;
    unsigned int    unrenderedCounter;
    unsigned int    frameCounter;
} BlinkAnimation;

SpriteAnimation
CreateSpriteAnimation(int totalFrames, int verticalFrames, int horizontalFrames, int frameSpeed, int sheetWidth, int sheetHeight);

void
DrawAnimationFrame(Texture2D *spritesheet, SpriteAnimation *spriteAnimation, Vector2 *spritePosition, int gameFPS);

void
DrawBlinkAnimation(Texture2D *spritesheet, BlinkAnimation *blinkAnimation, Vector2 *spritePosition, int gameFPS);

BlinkAnimation
CreateBlinkAnimation(int blinksPerSecond);
