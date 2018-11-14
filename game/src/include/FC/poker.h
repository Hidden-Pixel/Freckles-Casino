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

typedef enum {
    PokerHand_HighCard = 0,
    PokerHand_Pair,
    PokerHand_TwoPair,
    PokerHand_ThreeOfAKind,
    PokerHand_Straight,
    PokerHand_Flush,
    PokerHand_FullHouse,
    PokerHand_FourOfAKind,
    PokerHand_StraightFlush
} Poker_Hand;

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
    Poker_Hand player_hand_type;
    Poker_Hand dealer_hand_type;
    int chances_left;
    int player_score;
    int dealer_score;
} Poker_Game;

Poker_Hand
Poker_FindAllHands(Poker_Card* player_hand, Poker_Card* house_cards, int house_card_count);

void
Poker_Init();

Poker_Card
Poker_DrawOne(Poker_CardState state);

void
Poker_StartNewRound(Poker_Game* game_state);

void
Poker_ProcessNewState(Poker_Game* game_state);

void
Poker_Update(Poker_Game* game_state);

/* Linked list */
struct node {
  Poker_Card value;
  struct node* next;
};

typedef struct node Poker_LinkedListNode;

typedef struct {
    Poker_LinkedListNode* first;
    Poker_LinkedListNode* last;
} Poker_CardList;

Poker_CardList*
Poker_CreateCardList(Poker_Card first_card);

void
Poker_AddCardToList(Poker_CardList* card_list, Poker_Card card);

void
Poker_DestroyCardList(Poker_CardList* card_list);

