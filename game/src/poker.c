#include <FC/poker.h>
#include <FC/buffer.h>
#include <FC/commands.h>

#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <memory.h>

// There's 52 cards in a deck and we're NOT counting jokers, folks.
#define DECK_SIZE 52
#define MAX_HAND_COMBOS 22
#define MAX_HOLDS 5
#define DEFAULT_ANTE 25
#define STRAIGHT_HANDS 9

// TODO: populate with possible straights.
local_persist int Straights[STRAIGHT_HANDS];

local_persist Poker_Card DealersDeck[DECK_SIZE];
local_persist Poker_Card SampleDeck[DECK_SIZE];
local_persist int deck_index = 0;
local_persist Buffer hands_buffer =
{
    .capacity       = 0,
    .size           = 0,
    .element_size   = 0,
    .memory         = NULL
};

Poker_Card
Init_Poker_Card()
{
    return (Poker_Card)
    {
        .suit       = CardSuit_None,
        .face_value = CardFace_None,
        .state      = CardState_None,
        .hold       = 0,
    };
}

void
Poker_CacheHands()
{
    // Create all values for hands with a straight in them
    for (int i = 0; i < STRAIGHT_HANDS; ++i) {
        // NOTE: Assumes little endian
        int straight = 0;
        for (int j = i; j < i + 5; ++j) {
            straight |= (1 << j);
        }
        Straights[i] = straight;
    }
}

internal void
Poker_ToggleCardHold(Poker_Card* card_hand, int card_index, unsigned char hold_state) {
    card_hand[card_index].hold = hold_state;
}

internal void
Poker_FiveCard_FinishCardHold(Poker_Card* card_hand, Poker_CardState card_visibility, int hand_size) {
    for (int i = 0; i < hand_size; ++i) {
        Poker_Card card = card_hand[i];
        if (card.hold == HoldState_NotHeld) {
            card_hand[i] = Poker_DrawOne(card_visibility);
        }
    }
}

void
Poker_Init(Poker_Game *game_state, Poker_GameType game_type)
{
    game_state->poker_type = game_type;
    game_state->poker_state = PokerState_NotStarted;
    switch (game_state->poker_type)
    {
        case GameType_FiveCard:
        {
            Poker_Init_FiveCard(game_state);
        } break;

        case GameType_Holdem:
        {
            Poker_Init_Holdem(game_state);
        } break;
    }
    for (int i = 0; i < DECK_SIZE; ++i)
    {
            SampleDeck[i].suit = (Poker_CardSuit)(i % CardSuit_Count);
            SampleDeck[i].face_value = (Poker_CardFace)(i % CardFace_Count);
            SampleDeck[i].state = CardState_Hidden;
    }
    srand(time(NULL));
    Poker_CacheHands();
    Command_OnCardHoldPressed = &Poker_ToggleCardHold;
    Command_OnCardHoldComplete = &Poker_FiveCard_FinishCardHold;
}

internal inline void
Poker_Init_FiveCard(Poker_Game *game_state)
{
    for (int i = 0; i < len(game_state->player_hand); i++)
    {
        game_state->player_hand[i] = Init_Poker_Card();
        game_state->dealer_hand[i] = Init_Poker_Card();
    }
    game_state->player_hand_type = PokerHand_None;
    game_state->dealer_hand_type = PokerHand_None;
    game_state->player_score = 1000;
    game_state->dealer_score = 1000;
    game_state->betting_round = 0;
    game_state->chances_left = 3;
}

internal inline void
Poker_Init_Holdem(Poker_Game *game_state)
{
    // player_hand
    game_state->player_hand[0] = Init_Poker_Card();
    game_state->player_hand[1] = Init_Poker_Card();
    // dealer_hand
    game_state->dealer_hand[0] = Init_Poker_Card();
    game_state->dealer_hand[1] = Init_Poker_Card();
    // house_hand
    game_state->house_hand[0] = Init_Poker_Card();
    game_state->house_hand[1] = Init_Poker_Card();
    game_state->house_hand[2] = Init_Poker_Card();
    game_state->house_hand[3] = Init_Poker_Card();
    game_state->house_hand[4] = Init_Poker_Card();
    // player_hand_type
    game_state->player_hand_type = PokerHand_None;
    game_state->dealer_hand_type = PokerHand_None;
    game_state->chances_left = 0;
    game_state->player_score = 0;
    game_state->dealer_score = 0;
}

int
Poker_CardRank(Poker_Card card) {
    return card.suit * 13 + card.face_value;
}

Poker_Hand
Poker_FindBestHand(Poker_Card* player_hand, int hand_size)
{
    int card_counts[CardFace_Count] = { 0 };
    int suit_counts[CardSuit_Count] = { 0 };
    int hand_flags[PokerHand_Count] = { 0 };
    int hand_bits = 0;

    for (int i = 0; i < hand_size; ++i) {
        card_counts[player_hand[i].face_value] += 1;
        suit_counts[player_hand[i].suit] += 1;
        // 13 bit flags for cards in hand.
        // 0 0000 0001 1111 would be a low straight.
        hand_bits |= (1 << player_hand[i].face_value);
    }

    for (int i = 0; i < CardSuit_Count; ++i) {
        if (suit_counts[i] == 5) {
            hand_flags[PokerHand_Flush] += 1;
            break;
        }
    }

    for (int i = 0; i < STRAIGHT_HANDS; ++i) {
        if (Straights[i] == hand_bits) {
            hand_flags[PokerHand_Straight] += 1;
            break;
        }
    }

    // Figure out if we have a straight flush or royal flush
    if (hand_flags[PokerHand_Straight] > 0 && hand_flags[PokerHand_Flush] > 0) {
        if (card_counts[CardFace_Ace] > 0) {
            return PokerHand_RoyalFlush;
        }

        return PokerHand_StraightFlush;
    }

    if (hand_flags[PokerHand_Flush] > 0) {
        return PokerHand_Flush;
    }

    if (hand_flags[PokerHand_Straight] > 0) {
        return PokerHand_Straight;
    }

    for (int i = 0; i < CardFace_Count; ++i) {
        if (card_counts[i] == 2) {
            hand_flags[PokerHand_Pair] += 1;
        }
        if (card_counts[i] == 3) {
            hand_flags[PokerHand_ThreeOfAKind] += 1;
        }
        if (card_counts[i] == 4) {
            hand_flags[PokerHand_FourOfAKind] += 1;
        }
    }

    if (hand_flags[PokerHand_Pair] > 0 && hand_flags[PokerHand_ThreeOfAKind] > 0) {
        return PokerHand_FullHouse;
    }

    if (hand_flags[PokerHand_FourOfAKind] > 0) {
        return PokerHand_FourOfAKind;
    }

    if (hand_flags[PokerHand_ThreeOfAKind] > 0) {
        return PokerHand_ThreeOfAKind;
    }

    if (hand_flags[PokerHand_Pair] == 2) {
        return PokerHand_TwoPair;
    }

    if (hand_flags[PokerHand_Pair] > 0) {
        return PokerHand_Pair;
    }

    return PokerHand_HighCard;
}

Poker_Card
Poker_DrawOne(Poker_CardState state)
{
    assert(deck_index < DECK_SIZE);
    Poker_Card card = DealersDeck[deck_index];
    card.state = state;
    deck_index++;
    return card;
}

internal void
Poker_Shuffle(Poker_Game *game_state)
{
    deck_index = 0;
    for (int i = 0; i < DECK_SIZE; ++i) 
    {
        DealersDeck[i] = SampleDeck[i];
    }
    for (int shuffles = 0; shuffles < 5; ++shuffles) 
    {
        // Fisher-Yates shuffle
        for (int i = 51; i > 1; --i) 
        {
            int j = rand() % i;
            Poker_Card temp = DealersDeck[i];
            DealersDeck[i] = DealersDeck[j];
            DealersDeck[j] = temp;
        }
    }
    game_state->poker_state = PokerState_Shuffled;
}

internal void
Poker_DealCards(Poker_Game *game_state)
{
    int i = 0;
    switch (game_state->poker_type)
    {
        case GameType_FiveCard:
        {
            i = 5;
        } break;

        case GameType_Holdem:
        {
            i = 2;
        } break;
    }
    while (i >= 0)
    {
        game_state->player_hand[i] = Poker_DrawOne(CardState_Shown);
        game_state->dealer_hand[i] = Poker_DrawOne(CardState_Hidden);
        i--;
    }
    if (game_state->poker_type == GameType_Holdem)
    {
        for (int j = 0; j < 5; ++j)
        {
            game_state->house_hand[i].state = CardState_Hidden;
        }
    }

    game_state->poker_state = PokerState_PlayerCardsDealt;
}

void
Poker_RevealHand(Poker_Card* hand, int hand_size) {
    for (int i = 0; i < hand_size; ++i) {
        hand[i].state = CardState_Shown;
    }
}

void
Poker_StartNewRound(Poker_Game *game_state)
{
    game_state->poker_state = PokerState_Started;
    Poker_Shuffle(game_state);
    Poker_DealCards(game_state);
    game_state->betting_round = 0;
}

void
Poker_ProcessState(Poker_Game *game_state) 
{
    switch (game_state->poker_type)
    {
        case GameType_FiveCard:
        {
            Poker_ProcessFiveCardState(game_state);
        } break;

        case GameType_Holdem:
        {
            Poker_ProcessHoldemState(game_state);
        } break;
    }
}

internal void
Poker_ProcessFiveCardState(Poker_Game *game_state)
{
    switch (game_state->poker_state)
    {
        case PokerState_Started:
        {
            if (game_state->player_score > DEFAULT_ANTE)
            {
                Poker_StartNewRound(game_state);
                game_state->player_score -= DEFAULT_ANTE;
            } 
            else
            {
                game_state->poker_state = PokerState_GameOver;
            }
        } break;

        case PokerState_PlayerCardsDealt:
        {
            game_state->poker_state = PokerState_Betting;
            game_state->betting_round++;
        } break;

        case PokerState_SelectHolds:
        {
            // TODO(nick): ...
        } break;

        case PokerState_Betting:
        {
            // TODO(nick): start betting process ...
            if (game_state->betting_round == 1)
            {
                // TODO(nick):
                game_state->poker_state = PokerState_SelectHolds;
            }
            else
            {
                // TODO(nick):
            }
        } break;

        case PokerState_ExchangeCards:
        {
            // TODO(nick): only allow one betting round ...
        } break;

        case PokerState_GameOver:
        {
            // TODO(nick): ...
        } break;
    }
}

internal void
Poker_ProcessHoldemState(Poker_Game *game_state)
{
    switch (game_state->poker_state)
    {
        case PokerState_NotStarted:
        {
            Poker_StartNewRound(game_state);
        } break;

        case PokerState_Shuffled:
        {
            for(int i = 0; i < 2; ++i)
            {
                // TODO: Card dealing / flipping animation
                game_state->player_hand[i] = Poker_DrawOne(CardState_Shown);
                game_state->dealer_hand[i] = Poker_DrawOne(CardState_Shown);
            }
            game_state->poker_state = PokerState_PlayerCardsDealt;
        } break;

        case PokerState_PlayerCardsDealt:
        {
            for (int i = 0; i < 3; ++i)
            {
                game_state->house_hand[i] = Poker_DrawOne(CardState_Shown);
            }
            game_state->poker_state = PokerState_FlopCardsDealt;
        } break;

        case PokerState_FlopCardsDealt:
        {
            game_state->house_hand[3] = Poker_DrawOne(CardState_Shown);
            game_state->poker_state = PokerState_RiverCardsDealt;
        } break;

        case PokerState_RiverCardsDealt:
        {
            game_state->house_hand[4] = Poker_DrawOne(CardState_Shown);
            game_state->poker_state = PokerState_TurnCardsDealt;
        } break;

        case PokerState_TurnCardsDealt:
        {
            // TODO: Award / Deduct points.
            // TODO: Lose / Win animations and sounds.
            game_state->poker_state = PokerState_GameOver;
        } break;

        case PokerState_GameOver:
        {
            // TODO: Continue / New Game / Quit
            game_state->poker_state = PokerState_NotStarted;
        } break;

        default:
        {

        } break;
    }
}

void
Poker_Update(Poker_Game* game_state) 
{
    // TODO(nick):
}

Poker_CardList* Poker_CreateCardList(Poker_Card first_card) 
{
    Poker_CardList* list = (Poker_CardList*)malloc(sizeof(Poker_CardList));
    list->first = (Poker_LinkedListNode*)malloc(sizeof(Poker_LinkedListNode));
    list->first->value = first_card;
    list->first->next = NULL;
    list->last = list->first;
    return list;
}

void Poker_AddCardToList(Poker_CardList* card_list, Poker_Card card)
{
    card_list->last->next = (Poker_LinkedListNode*)malloc(sizeof(Poker_LinkedListNode));
    card_list->last = card_list->last->next;
    card_list->last->value = card;
    card_list->last->next = NULL;
}

void Poker_DestroyCardList(Poker_CardList* card_list)
{
    while(card_list->first) 
    {
        Poker_LinkedListNode* p_node = card_list->first;
        card_list->first = card_list->first->next;
        free(p_node);
    }

    free(card_list);
}
