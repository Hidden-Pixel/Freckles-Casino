/*
 *  character-state.h
 *
 */

typedef enum _mrFrecklesState
{
    MrFrecklesIdle              = 0x00,
    MrFrecklesDealing           = 0x01,
    MrFrecklesTalkingNormal     = 0x02,
    MrFrecklesTalkingAngry      = 0x03,
    MrFrecklesWinning           = 0x04,
} MrFrecklesState;
