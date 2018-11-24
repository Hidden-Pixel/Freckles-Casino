/*
 *  input.c
 *
 */

#include <FC/input.h>

void
ProcessInput(Poker_Game* game_state)
{
    local_persist bool confirmPressed = false;

    // TODO(nick): clean this up - just testing code
    /*
    if (IsKeyPressed(KEY_UP) && confirmPressed == false)
    {
        if (CurrentCharacterId == MrFreckles)
        {
            if (MrFrecklesActiveState >= Losing)
            {
                MrFrecklesActiveState = Idle;
            }
            else
            {
                MrFrecklesActiveState++;
            }
        }
        confirmPressed = true;
    }

    if (IsKeyDown(KEY_S) && confirmPressed == false && GameScene == SceneTitleScreen)
    {
        GameScene = SceneMainPokerTable;
    }

    if (IsKeyUp(KEY_S) && confirmPressed == true)
    {
        confirmPressed = false;
    }

    if (IsKeyUp(KEY_UP) && confirmPressed == true)
    {
        confirmPressed = false;
    }
    
    if (IsKeyDown(KEY_SPACE) && confirmPressed == false)
    {
        confirmPressed = true;
        HandleConfirmButtonPress(game_state);
    }
    
    if (IsKeyUp(KEY_SPACE) && confirmPressed == true)
    {
        confirmPressed = false;
    }
    */
}
