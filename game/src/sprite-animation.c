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
    spriteFramePosition.x = (horizontalOffset * (spritesheet->width / spriteAnimation->totalHorizontalFrames));
    spriteFramePosition.y = (verticalOffset * (spritesheet->height / spriteAnimation->totalVerticalFrames));
    spriteAnimation->currentFrameRect = (Rectangle)
    {
        .x = spriteFramePosition.x,
        .y = spriteFramePosition.y,
        .width = (spritesheet->width / spriteAnimation->totalHorizontalFrames),
        .height = (spritesheet->height / spriteAnimation->totalVerticalFrames),
    };
    DrawTextureRec(*spritesheet, spriteAnimation->currentFrameRect, *spritePosition, WHITE);
    spriteAnimation->frameCounter++;
}
