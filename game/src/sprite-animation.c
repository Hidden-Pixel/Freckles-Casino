/*
 *  sprite-animation.c
 *
 */

/*
typedef enum _spritesheetPosition
{
    Undefined   = 0x00,
    Horizontal  = 0x01,
    Vertical    = 0x02,
} SpritesheetPosition;
*/

typedef struct _spriteAnimation 
{
    unsigned int    totalFrames;
    unsigned int    totalVerticalFrames;
    unsigned int    totalHorizontalFrames;
    unsigned int    currentDrawFrameIndex;
    unsigned int    frameCounter;
    unsigned int    frameSpeed;
    unsigned char   frameOrder;
    Rectangle       currentFrameRect;
} SpriteAnimation;


void
DrawAnimationFrame(Texture2D *spritesheet, SpriteAnimation *spriteAnimation, Vector2 *spritePosition, int gameFPS)
{
    if (spriteAnimation->frameCounter >= (gameFPS / spriteAnimation->frameSpeed))
    {
        spriteAnimation->frameCounter = 0;
        if (spriteAnimation->currentDrawFrameIndex > (spriteAnimation->totalFrames - 1))
        {
            spriteAnimation->currentDrawFrameIndex = 0;
        }
        else
        {
            spriteAnimation->currentDrawFrameIndex++;
        }
    }
    Vector2 spriteFramePosition =
    {
        .x = 0,
        .y = 0,
    };
    int verticalOffset = floor(spriteAnimation->currentDrawFrameIndex / spriteAnimation->totalVerticalFrames);
    spriteFramePosition.y = floor(verticalOffset * (spritesheet->height / spriteAnimation->totalVerticalFrames));
    int horizontalOffset = floor((spriteAnimation->currentDrawFrameIndex % spriteAnimation->totalHorizontalFrames));
    spriteFramePosition.x = floor(horizontalOffset * (spritesheet->width / spriteAnimation->totalHorizontalFrames));
    spriteAnimation->currentFrameRect = (Rectangle)
    {
        .x = spriteFramePosition.x,
        .y = spriteFramePosition.y,
        .width = floor(spritesheet->width / spriteAnimation->totalHorizontalFrames),
        .height = floor(spritesheet->height / spriteAnimation->totalVerticalFrames),
    };
    DrawTextureRec(*spritesheet, spriteAnimation->currentFrameRect, *spritePosition, WHITE);
    spriteAnimation->frameCounter++;
}
