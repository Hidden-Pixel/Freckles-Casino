#include <FC/poker.h>
#include <FC/buffer.h>

#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <memory.h>

// There's 52 cards in a deck and we're NOT counting jokers, folks.
#define DECK_SIZE 52
#define MAX_HAND_COMBOS 22

local_persist Poker_Card DealersDeck[DECK_SIZE];
local_persist Poker_Card SampleDeck[DECK_SIZE];
local_persist int deck_index = 0;
local_persist Buffer hands_buffer =
{
    .size           = 0,
    .capacity       = 0,
    .element_size   = 0,
    .memory         = NULL,
};

Poker_Card
Init_Poker_Card()
{
    return (Poker_Card)
    {
        .suit       = CardSuit_None,
        .face_value = CardFace_None,
        .state      = CardState_None,
    };
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
            SampleDeck[i].suit = (Poker_CardSuit)((i % CardSuit_Count) + 1);
            SampleDeck[i].face_value = (Poker_CardFace)((i % CardFace_Count) + 2);
            SampleDeck[i].state = CardState_Hidden;
    }
    srand(time(NULL));
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
    game_state->player_score = 0;
    game_state->dealer_score = 0;
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

/*
 * @remark We're assuming that player_hand is length 2
 */
Poker_Hand
Poker_FindAllHands(Poker_Card* player_hand, Poker_Card* house_cards, int house_card_count) 
{
    int card_count_squared = pow(2, house_card_count);
    if (house_card_count > 2) {
        // combinations: f(x) = h(h-x), where h is the house count and x is the size of picks
        int three_house_cards = house_card_count > 3 ? card_count_squared - house_card_count * 3 : 1;
        // We'll pre-calculate these, at least until we decide we want to support Omaha
        int four_house_cards = house_card_count > 4 ? 5 : 0;
        int five_house_cards = house_card_count > 4 ? 1 : 0;
        int total_combinations = three_house_cards + four_house_cards + five_house_cards;
        // Create all the possible hands
        // TODO(nick): replace with list - IMPORTANT
        if (hands_buffer.memory == NULL)
        {
            Buffer_Create(sizeof(Poker_CardList *), total_combinations);
        }
        //Poker_CardList* hands[MAX_HAND_COMBOS];
        // TODO: Create all possible hands
    } else {
      // TODO: Just process the player's hand.
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
Deal_Cards(Poker_Game *game_state)
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
        for (int i = 0; i < 5; ++i) 
        {
            game_state->house_hand[i].state = CardState_Hidden;
        }
    }
    game_state->poker_state = PokerState_PlayerCardsDealt;
}

void
Poker_StartNewRound(Poker_Game *game_state)
{
    game_state->poker_state = PokerState_Started;
    Poker_Shuffle(game_state);
    for (int i = 0; i < 2; ++i) 
    {
        game_state->player_hand[i] = Poker_DrawOne(CardState_Shown);
        game_state->dealer_hand[i] = Poker_DrawOne(CardState_Hidden);
    }
    for (int i = 0; i < 5; ++i) 
    {
        game_state->house_hand[i].state = CardState_Hidden;
    }
    game_state->poker_state = PokerState_PlayerCardsDealt;
}

void
Poker_ProcessNewState(Poker_Game *game_state) 
{
    switch (game_state->poker_type)
    {
        case GameType_FiveCard:
        {
            Poker_ProcessNewFiveCardState(game_state);
        } break;

        case GameType_Holdem:
        {
            Poker_ProcessNewHoldemState(game_state);
        } break;
    }
}

internal void
Poker_ProcessNewFiveCardState(Poker_Game *game_state)
{
    // TODO(nick):
}

internal void
Poker_ProcessNewHoldemState(Poker_Game *game_state)
{
    switch (game_state->poker_state)
    {
        case PokerState_NotStarted:
        {
            Poker_StartNewRound(game_state);
            game_state->poker_state = PokerState_Shuffled;
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
