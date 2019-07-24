/*
 *  sprite-animation.c
 *
 */

#include <FC/font-info.h>
#include <FC/sprite-animation.h>
#include <cstdint>
#include <limits>

typedef enum _spritesheetPosition
{
    Undefined   = 0x00,
    Horizontal  = 0x01,
    Vertical    = 0x02,
} SpritesheetPosition;

SpriteAnimation
CreateSpriteAnimation(unsigned int totalFrames,
        unsigned int verticalFrames,
        unsigned int horizontalFrames,
        unsigned int frameSpeed,
        int sheetWidth, int sheetHeight)
{
    SpriteAnimation spriteAnimation = (SpriteAnimation)
    {
        .currentDrawFrameIndex  = 0,
        .frameCounter           = 0,
        .totalFramesRendered    = 0,
        .totalFrames            = totalFrames,
        .totalVerticalFrames    = verticalFrames,
        .totalHorizontalFrames  = horizontalFrames,
        .frameSpeed             = frameSpeed,
        .frameWidth             = ((float)sheetWidth / (float)horizontalFrames),
        .frameHeight            = ((float)sheetHeight / (float)verticalFrames),
        .pauseFrame             = std::numeric_limits<unsigned int>::max(),
        .pauseFrameDuration     = 0,
        .paused                 = false,
    };
    return spriteAnimation;
}

SpriteAnimation
CreateSpriteAnimationWithPause(unsigned int totalFrames,
        unsigned int verticalFrames,
        unsigned int horizontalFrames,
        unsigned int frameSpeed,
        int sheetWidth, int sheetHeight,
        unsigned int pauseFrame,
        unsigned int pauseFrameDuration)
{
    SpriteAnimation spriteAnimation = (SpriteAnimation)
    {
        .currentDrawFrameIndex  = 0,
        .frameCounter           = 0,
        .totalFramesRendered    = 0,
        .totalFrames            = totalFrames,
        .totalVerticalFrames    = verticalFrames,
        .totalHorizontalFrames  = horizontalFrames,
        .frameSpeed             = frameSpeed,
        .frameWidth             = ((float)sheetWidth / (float)horizontalFrames),
        .frameHeight            = ((float)sheetHeight / (float)verticalFrames),
        .pauseFrame             = pauseFrame,
        .pauseFrameDuration     = pauseFrameDuration,
        .paused                 = false,
    };
    return spriteAnimation;
}

void
DrawAnimationFrame(Texture2D *spritesheet, SpriteAnimation *spriteAnimation, Vector2 *spritePosition, int gameFPS)
{
    if (spriteAnimation->frameCounter >= (gameFPS / spriteAnimation->frameSpeed))
    {
        spriteAnimation->frameCounter = 0;
        spriteAnimation->totalFramesRendered++;
        spriteAnimation->currentDrawFrameIndex++;
        if (spriteAnimation->currentDrawFrameIndex == spriteAnimation->pauseFrame)
        {
            spriteAnimation->paused = true;
            if ((spriteAnimation->totalFramesRendered % spriteAnimation->pauseFrameDuration) == 0)
            {
                spriteAnimation->paused = false;
            }
        }
        if (spriteAnimation->paused == true)
        {
            spriteAnimation->currentDrawFrameIndex = (spriteAnimation->pauseFrame - 1);
        }
        else if (spriteAnimation->currentDrawFrameIndex > (spriteAnimation->totalFrames - 1))
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

BlinkAnimation
CreateBlinkAnimation(unsigned int blinksPerSecond)
{
    BlinkAnimation blinkAnimation = (BlinkAnimation)
    {
        .blinksPerSecond        = blinksPerSecond,
        .blinkDurationFrames    = 0,
        .frameCounter           = 0,
    };
    return blinkAnimation;
}

void
DrawBlinkAnimation(void *asset, AssetType assetType, BlinkAnimation *blinkAnimation, Vector2 *position, int gameFPS) 
{
    int frameBlink = (gameFPS / blinkAnimation->blinksPerSecond);
    blinkAnimation->frameCounter++;
    if (blinkAnimation->frameCounter >= frameBlink)
    {
        blinkAnimation->blinkDurationFrames++;
        if (blinkAnimation->blinkDurationFrames >= (frameBlink / 4))
        {
            blinkAnimation->blinkDurationFrames = 0;
            blinkAnimation->frameCounter = 0;
        }
    }
    if (blinkAnimation->blinkDurationFrames == 0)
    {
        switch (assetType)
        {
            case AssetType_Texture2D:
            {
                DrawTexture(*((Texture2D *)asset), position->x, position->y, WHITE);
            } break;

            case AssetType_Text:
            {
                FontInfo *fontInfo = ((FontInfo *)asset);
                DrawTextEx(fontInfo->Font, fontInfo->Text, *position, fontInfo->Size, fontInfo->SpacingSize, fontInfo->Color);
            } break;

            default:
            {
                // TODO(nick): logging - bad!
            } break;
        }
    }
}

FadeAnimation
CreateFadeAnimation(unsigned int frameRenderDurationSeconds, unsigned int fadeInDurationFrames, unsigned int fadeOutDurationFrames)
{
    FadeAnimation fadeAnimation = (FadeAnimation)
    {
        .frameRenderDurationSeconds = frameRenderDurationSeconds,
        .fadeInDurationFrames       = fadeInDurationFrames,
        .fadeOutDurationFrames      = fadeOutDurationFrames,
        .fadeCurrentAlphaValue      = 0.0f,
        .frameCounter               = 0,
    };
    return fadeAnimation;
}

void
DrawFadeAnimation(void *asset, AssetType assetType, FadeAnimation *fade, Vector2 *position, int gameFPS)
{
    switch(assetType) 
    {
        case AssetType_Texture2D:
        {
            if (fade->frameCounter <= fade->fadeInDurationFrames)
            {
                DrawTexture(*((Texture2D *)asset), position->x, position->y, Fade(WHITE, fade->fadeCurrentAlphaValue));
                // TODO(nick): actually compute the values in the CreateFadeAnimation function ...
                fade->fadeCurrentAlphaValue += 0.01f;
            }
            else if (fade->frameCounter <= fade->fadeOutDurationFrames)
            {
                DrawTexture(*((Texture2D *)asset), position->x, position->y, Fade(WHITE, fade->fadeCurrentAlphaValue));
                // TODO(nick): actually compute the values in the CreateFadeAnimation function ...
                fade->fadeCurrentAlphaValue -= 0.01f;
            }
        } break;

        default:
        {
            // TODO(nick): logging - bad!
        } break;
    }
    fade->frameCounter++;
}
