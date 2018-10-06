#pragma once

typedef enum {
    Poker_Heart = 0,
    Poker_Club,
    Poker_Diamond,
    Poker_Spade,
    Poker_SuitCount
} Poker_CardSuit;

typedef enum {
    Poker_Two = 2,
    Poker_Three,
    Poker_Four,
    Poker_Five,
    Poker_Six,
    Poker_Seven,
    Poker_Eight,
    Poker_Nine,
    Poker_Ten,
    Poker_Jack,
    Poker_Queen,
    Poker_King,
    Poker_Ace,
    Poker_FaceCount = 13
} Poker_CardFace;

typedef struct {
    Poker_CardSuit suit;
    Poker_CardFace face_value;
} Poker_Card;

void
Poker_Init();

Poker_Card
Poker_DrawOne();

void
Poker_StartNewRound();


