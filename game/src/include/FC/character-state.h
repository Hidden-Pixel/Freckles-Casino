/*
 *  character-state.h
 *
 */

typedef enum _characterId
{
    MrFreckles           = 0x00,
} CharcterId;

typedef enum _characterState
{
    Idle                = 0x00,
    GamePlay            = 0x01,
    GamePlayFinal_1     = 0x02,
    GamePlayFinal_2     = 0x03,
    IdleStress_1        = 0x04,
    IdleStress_2        = 0x05,
    IdleStress_3        = 0x06,
    TalkStress_1        = 0x07,
    TalkStress_2        = 0x08,
    TalkStress_3        = 0x09,
    TalkStressFinal     = 0x0A,
    Win                 = 0x0B,
    Lose                = 0x0C,

    // TODO(nick): remove / replace these
    /*
    PlayingCards      = 0x01,
    Winning           = 0x02,
    Losing            = 0x03,
    */
} CharacterState;
