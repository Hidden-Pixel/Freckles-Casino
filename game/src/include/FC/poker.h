#pragma once

#include <FC/defines.h>

#define CardFace_Count 13
#define CardSuit_Count  4

typedef enum _poker_CardSuit
{
    CardSuit_None       =  0,
    CardSuit_Heart      =  1,
    CardSuit_Club       =  2,
    CardSuit_Diamond    =  3,
    CardSuit_Spade      =  4,
} Poker_CardSuit;

typedef enum poker_CardFace
{
    CardFace_None   =  0,
    CardFace_Two    =  2,
    CardFace_Three  =  3,
    CardFace_Four   =  4,
    CardFace_Five   =  5,
    CardFace_Six    =  6,
    CardFace_Seven  =  7,
    CardFace_Eight  =  8,
    CardFace_Nine   =  9,
    CardFace_Ten    = 10,
    CardFace_Jack   = 11,
    CardFace_Queen  = 12,
    CardFace_King   = 13,
    CardFace_Ace    = 14,
} Poker_CardFace;

typedef enum poker_GameType
{
    GameType_None       = 0,
    GameType_FiveCard   = 1,
    GameType_Holdem     = 2,
} Poker_GameType;

typedef enum _poker_CardState
{
    CardState_None      = 0,
    CardState_Hidden    = 1,
    CardState_Shown     = 2,
} Poker_CardState;

typedef enum _poke_GameState
{
    PokerState_NotStarted       = 0,
    PokerState_Started          = 1,
    PokerState_Shuffled         = 2,
    PokerState_PlayerCardsDealt = 3,
    PokerState_FlopCardsDealt   = 4,
    PokerState_RiverCardsDealt  = 5,
    PokerState_TurnCardsDealt   = 6,
    PokerState_SelectHolds      = 7,
    PokerState_Betting          = 8,
    PokerState_GameOver         = 9,
    PokerState_ExchangeCards    = 10,
    PokerState_Count            = 11,
} Poker_GameState;

typedef enum _poker_Hand
{
    PokerHand_None          = -1,
    PokerHand_HighCard      =  0,
    PokerHand_Pair          =  1,
    PokerHand_TwoPair       =  2,
    PokerHand_ThreeOfAKind  =  3,
    PokerHand_Straight      =  4,
    PokerHand_Flush         =  5,
    PokerHand_FullHouse     =  6,
    PokerHand_FourOfAKind   =  7,
    PokerHand_StraightFlush =  8,
} Poker_Hand;

typedef struct _poker_Card
{
    Poker_CardSuit  suit;
    Poker_CardFace  face_value;
    Poker_CardState state;
    unsigned char   hold;
} Poker_Card;

typedef struct _poker_Game
{
    Poker_GameState poker_state;
    Poker_GameType  poker_type;
    Poker_Card      player_hand[5];
    Poker_Card      dealer_hand[5];
    Poker_Card      house_hand[5];
    Poker_Hand      player_hand_type;
    Poker_Hand      dealer_hand_type;
    int             player_hold_count;
    int             chances_left;
    int             player_score;
    int             dealer_score;
} Poker_Game;

void
Poker_Init(Poker_Game *game_state, Poker_GameType game_type);

internal inline void
Poker_Init_FiveCard(Poker_Game *game_state);

internal inline void
Poker_Init_Holdem(Poker_Game *game_state);

Poker_Hand
Poker_FindAllHands(Poker_Card *player_hand, Poker_Card *house_cards, int house_card_count);

Poker_Card
Poker_DrawOne(Poker_CardState state);

internal void
Poker_Shuffle(Poker_Game *game_state);

internal void
Poker_DealCards(Poker_Game *game_state);

void
Poker_StartNewRound(Poker_Game *game_state);

void
Poker_ProcessState(Poker_Game* game_state);

internal void
Poker_ProcessFiveCardState(Poker_Game *game_state);

internal void
Poker_ProcessHoldemState(Poker_Game *game_state);

void
Poker_Update(Poker_Game* game_state);

/* Linked list */
struct node 
{
  Poker_Card value;
  struct node* next;
};

typedef struct node Poker_LinkedListNode;

typedef struct 
{
    Poker_LinkedListNode* first;
    Poker_LinkedListNode* last;
} Poker_CardList;

Poker_CardList*
Poker_CreateCardList(Poker_Card first_card);

void
Poker_AddCardToList(Poker_CardList* card_list, Poker_Card card);

void
Poker_DestroyCardList(Poker_CardList* card_list);
