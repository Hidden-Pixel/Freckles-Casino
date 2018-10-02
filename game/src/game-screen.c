#include <FC/game-screen.h>

static float GameScreenUnit;
static Vector2 GameScreenScale;

void freckles::game_screen::Initialize(float screenWidth, float screenHeight, float gameWidth, float gameHeight) {
    GameScreenUnit = (screenWidth * screenHeight) / (gameWidth * gameHeight);
    GameScreenScale.x = screenWidth / gameWidth;
    GameScreenScale.y = screenHeight / gameHeight;
}

float
freckles::game_screen::LocalUnitsToScreen(float units)
{
    return (units * GameScreenUnit);
}

Vector2
freckles::game_screen::LocalToScreen(Vector2 local)
{
    return Vector2 { local.x * GameScreenScale.x, local.y * GameScreenScale.y };
}

float
freckles::game_screen::ScreenUnitScale() {
    return GameScreenUnit;
}