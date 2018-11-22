/*
 *  character-state.h
 *
 */

typedef enum _characterId
{
    MrFreckles           = 0x00,
    MrsFreckles          = 0x01,
    ColHindenburger      = 0x02,
    GeneralGrunt         = 0x03,
    Pyrella              = 0x04,
} CharcterId;

typedef enum _characterState
{
    Idle              = 0x00,
    PlayingCards      = 0x01,
    Winning           = 0x02,
    Losing            = 0x03,
} CharacterState;
