#pragma once

#include <raylib.h>

void
GameScreen_Init(float screenWidth, float screenHeight, float localWidth, float localHeight);

float
GameScreen_LocalUnitsToScreen(float units);

float
GameScreen_ScreenToLocalUnits(float size);

float
GameScreen_ScreenUnitScale();


