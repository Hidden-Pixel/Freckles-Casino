#pragma once

#include <raylib.h>

/*
 * Initializes the game screen with a proper scale for displaying the gameplay area.
 */
void
GameScreen_Init(float screenWidth, float screenHeight, float localWidth, float localHeight);

/*
 * Converts local game area units to the equivalent on the visible screen
 */
float
GameScreen_LocalUnitsToScreen(float units);

/*
 * Converts a visible screen area to the local game screen unit size
 * NOTE: This is the inverse of GameScreen_LocalUnitsToScreen
 */
float
GameScreen_ScreenAreaToLocalUnits(float size);

/*
 * Gets the game screen scale this was initialized with.
 */
float
GameScreen_ScreenUnitScale();
