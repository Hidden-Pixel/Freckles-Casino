#pragma once

#include <stdbool.h>

typedef enum {
    CardSuit_Heart = 0,
    CardSuit_Club,
    CardSuit_Diamond,
    CardSuit_Spade,
    CardSuit_Count
} Poker_CardSuit;

typedef enum {
    CardFace_Two = 2,
    CardFace_Three,
    CardFace_Four,
    CardFace_Five,
    CardFace_Six,
    CardFace_Seven,
    CardFace_Eight,
    CardFace_Nine,
    CardFace_Ten,
    CardFace_Jack,
    CardFace_Queen,
    CardFace_King,
    CardFace_Ace,
    CardFace_Count = 13
} Poker_CardFace;

typedef enum {
    CardState_None = 0,
    CardState_Hidden,
    CardState_Shown
} Poker_CardState;

typedef enum {
    PokerState_NotStarted = 0,
    PokerState_Shuffled,
    PokerState_PlayerCardsDealt,
    PokerState_FlopCardsDealt,
    PokerState_RiverCardsDealt,
    PokerState_TurnCardsDealt,
    PokerState_GameOver,
    PokerState_Count
} Poker_GameState;

typedef struct {
    Poker_CardSuit suit;
    Poker_CardFace face_value;
    Poker_CardState state;
} Poker_Card;

typedef struct {
    Poker_GameState poker_state;
    Poker_Card player_hand[2];
    Poker_Card dealer_hand[2];
    Poker_Card house_hand[5];
    int chances_left;
    int player_score;
    int dealer_score;
} Poker_Game;

void
Poker_Init();

Poker_Card
Poker_DrawOne(Poker_CardState state);

void
Poker_StartNewRound(Poker_Game* game_state);


