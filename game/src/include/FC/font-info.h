/*
 *  text.c
 *
 */

#include "raylib.h"

typedef struct _fontInfo
{
    Font  Font;
    float Size;
    float SpacingSize;
    Color Color;
    const char *Text;
} FontInfo;
