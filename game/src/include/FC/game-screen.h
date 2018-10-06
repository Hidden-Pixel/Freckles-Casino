#pragma once

#include <raylib.h>

void
Initialize(float screenWidth, float screenHeight, float localWidth, float localHeight);

float
LocalUnitsToScreen(float units);

float
ScreenToLocalUnits(float size);

float
ScreenUnitScale();
