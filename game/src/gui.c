/*
 * gui.c
 *
 */

#include <FC/gui.h>

GUI_Window
GUI_Window_Create(Vector2 coordinates, Vector2 dimensions)
{
    GUI_Window window  = (GUI_Window)
    {
        .Coordinates = coordinates,
        .Dimensions = dimensions
    };
    return window;
}
