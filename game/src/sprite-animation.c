/*
 *  sprite-animation.c
 *
 */

#include <FC/sprite-animation.h>

typedef enum _spritesheetPosition
{
    Undefined   = 0x00,
    Horizontal  = 0x01,
    Vertical    = 0x02,
} SpritesheetPosition;

SpriteAnimation
CreateSpriteAnimation(int totalFrames, int verticalFrames, int horizontalFrames, int frameSpeed, int sheetWidth, int sheetHeight)
{
    SpriteAnimation spriteAnimation = (SpriteAnimation)
    {
        .currentDrawFrameIndex  = 0,
        .frameCounter           = 0,
        .totalFrames            = totalFrames,
        .totalVerticalFrames    = verticalFrames,
        .totalHorizontalFrames  = horizontalFrames,
        .frameSpeed             = frameSpeed,
        .frameWidth             = ((float)sheetWidth / (float)horizontalFrames),
        .frameHeight            = ((float)sheetHeight / (float)verticalFrames),
    };
    return spriteAnimation;
}

void
DrawAnimationFrame(Texture2D *spritesheet, SpriteAnimation *spriteAnimation, Vector2 *spritePosition, int gameFPS)
{
    if (spriteAnimation->frameCounter >= (gameFPS / spriteAnimation->frameSpeed))
    {
        spriteAnimation->frameCounter = 0;
        spriteAnimation->currentDrawFrameIndex++;
        if (spriteAnimation->currentDrawFrameIndex > (spriteAnimation->totalFrames - 1))
        {
            spriteAnimation->currentDrawFrameIndex = 0;
        }
    }
    Vector2 spriteFramePosition =
    {
        .x = 0,
        .y = 0,
    };
    int verticalOffset = 0;
    int horizontalOffset = 0;
    verticalOffset = (spriteAnimation->currentDrawFrameIndex / spriteAnimation->totalHorizontalFrames);
    horizontalOffset = (spriteAnimation->currentDrawFrameIndex % spriteAnimation->totalHorizontalFrames);
    spriteFramePosition.x = (horizontalOffset * (spriteAnimation->frameWidth));
    spriteFramePosition.y = (verticalOffset * (spriteAnimation->frameHeight ));
    spriteAnimation->currentFrameRect = (Rectangle)
    {
        .x = spriteFramePosition.x,
        .y = spriteFramePosition.y,
        .width = spriteAnimation->frameWidth, 
        .height = spriteAnimation->frameHeight,
    };
    DrawTextureRec(*spritesheet, spriteAnimation->currentFrameRect, *spritePosition, WHITE);
    spriteAnimation->frameCounter++;
}
