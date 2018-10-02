#include <FC/game-screen.h>
#include <math.h>

static float GameScreenUnit;
static float ScreenToLocalUnit;
static Vector2 GameScreenScale;

void freckles::game_screen::Initialize(float screenWidth, float screenHeight, float gameWidth, float gameHeight) {
    GameScreenScale.x = screenWidth / gameWidth;
    GameScreenScale.y = screenHeight / gameHeight;
    GameScreenUnit = fmin(GameScreenScale.x, GameScreenScale.y);
    ScreenToLocalUnit = (1/GameScreenUnit);
}

float
freckles::game_screen::LocalUnitsToScreen(float units)
{
    return (units * GameScreenUnit);
}

float
ScreenToLocalUnits(float size)
{
    return (size * ScreenToLocalUnit);
}

float
freckles::game_screen::ScreenUnitScale() {
    return GameScreenUnit;
}