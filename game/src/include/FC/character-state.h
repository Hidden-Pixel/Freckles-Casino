/*
 *  character-state.h
 *
 */

typedef enum _characterId
{
    MrFreckles       = 0x00,
    MrsFreckles      = 0x01,
} CharcterId;

typedef enum _mrFrecklesState
{
    MrFrecklesIdle              = 0x00,
    MrFrecklesDealing           = 0x01,
    MrFrecklesTalkingNormal     = 0x02,
    MrFrecklesTalkingAngry      = 0x03,
    MrFrecklesWinning           = 0x04,
} MrFrecklesState;


typedef enum _mrsFrecklesState
{
    MrsFrecklesIdle              = 0x00,
    MrsFrecklesTalking           = 0x01,
    MrsFrecklesLosing            = 0x02,
    MrsFrecklesWinning           = 0x03,
} MrsFrecklesState;
