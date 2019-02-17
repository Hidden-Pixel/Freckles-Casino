#pragma once

#include <FC/defines.h>

#define CardFace_Count 13
#define CardSuit_Count  4
#define PokerHand_Count 10

extern const char* CardSuit_Names[CardSuit_Count];
extern const char* CardFace_ShortNames[CardFace_Count];
extern const char* CardFace_FullNames[CardFace_Count];
extern const char* Hand_Names[PokerHand_Count];

typedef enum _poker_CardSuit
{
    CardSuit_None       =  -1,
    CardSuit_Club       =  0,
    CardSuit_Diamond    =  1,
    CardSuit_Heart      =  2,
    CardSuit_Spade      =  3,
} Poker_CardSuit;

typedef enum poker_CardFace
{
    CardFace_None   =  -1,
    CardFace_Two    =  0,
    CardFace_Three  =  1,
    CardFace_Four   =  2,
    CardFace_Five   =  3,
    CardFace_Six    =  4,
    CardFace_Seven  =  5,
    CardFace_Eight  =  6,
    CardFace_Nine   =  7,
    CardFace_Ten    =  8,
    CardFace_Jack   =  9,
    CardFace_Queen  =  10,
    CardFace_King   =  11,
    CardFace_Ace    =  12,
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

typedef enum _poker_GameState
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
    PokerHand_RoyalFlush    =  9
} Poker_Hand;

typedef enum _poker_CardHoldState
{
    HoldState_NotHeld = 0,
    HoldState_Held = 2
} Poker_CardHoldState;

typedef struct _poker_Card
{
    Poker_CardSuit  suit;
    Poker_CardFace  face_value;
    Poker_CardState state;
    unsigned char   hold;
} Poker_Card;

// Performance Note: This is probably not well-aligned
// The cards could be pointers if it's ever a problem.
typedef struct _poker_RankedHand_5
{
    Poker_Hand hand_type;
    int card_counts[CardFace_Count];
    // 0 is the highest ranked card.
    Poker_Card ranked_cards[5];
} Poker_RankedHand_5;

typedef struct _poker_Game
{
    Poker_GameState poker_state;
    Poker_GameType  poker_type;
    Poker_Card      player_hand[5];
    Poker_Card      dealer_hand[5];
    Poker_Card      house_hand[5];
    Poker_Hand      player_hand_type;
    Poker_Hand      dealer_hand_type;
    int             betting_round;
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
Poker_RevealHand(Poker_Card* hand, int hand_size);

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

int
Poker_CardRank(Poker_Card card);

void
Poker_CacheHands();

Poker_CardList*
Poker_CreateCardList(Poker_Card first_card);

void
Poker_AddCardToList(Poker_CardList* card_list, Poker_Card card);

void
Poker_DestroyCardList(Poker_CardList* card_list);

Poker_Hand
Poker_FindBestHand(Poker_Card* player_hand, int hand_size);
