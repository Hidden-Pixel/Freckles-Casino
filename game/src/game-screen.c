#include <FC/game-screen.h>
#include <math.h>

static float GameScreenUnit;
static float ScreenToLocalUnit;
static Vector2 GameScreenScale;

void
GameScreen_Init(float screenWidth, float screenHeight, float gameWidth, float gameHeight)
{
    GameScreenScale.x = screenWidth / gameWidth;
    GameScreenScale.y = screenHeight / gameHeight;
    GameScreenUnit = fmin(GameScreenScale.x, GameScreenScale.y);
    ScreenToLocalUnit = (1/GameScreenUnit);
}

float
GameScreen_LocalUnitsToScreen(float units)
{
    return (units * GameScreenUnit);
}

float
GameScreen_ScreenToLocalUnits(float size)
{
    return (size * ScreenToLocalUnit);
}

float
GameScreen_ScreenUnitScale()
{
    return GameScreenUnit;
}
