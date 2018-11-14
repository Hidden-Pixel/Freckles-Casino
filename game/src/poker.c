#include <FC/poker.h>
#include <FC/defines.h>
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

/*
 * @remark We're assuming that player_hand is length 2
 */
Poker_Hand
Poker_FindAllHands(Poker_Card* player_hand, Poker_Card* house_cards, int house_card_count) {
    int card_count_squared = pow(2, house_card_count);

    if (house_card_count > 2) {
        // combinations: f(x) = h(h-x), where h is the house count and x is the size of picks
        int three_house_cards = house_card_count > 3 ? card_count_squared - house_card_count * 3 : 1;
        // We'll pre-calculate these, at least until we decide we want to support Omaha
        int four_house_cards = house_card_count > 4 ? 5 : 0;
        int five_house_cards = house_card_count > 4 ? 1 : 0;
        int total_combinations = three_house_cards + four_house_cards + five_house_cards;

        // Create all the possible hands
        Poker_CardList* hands[MAX_HAND_COMBOS];

        // TODO: Create all possible hands
        

    } else {
      // TODO: Just process the player's hand.

    }

    return PokerHand_HighCard;
}

Poker_Card
Poker_DrawOne(Poker_CardState state){
    assert(deck_index < DECK_SIZE);
    Poker_Card card = DealersDeck[deck_index];
    card.state = state;
    deck_index++;
    return card;
}

void
Poker_Init()
{
    for (int i = 0; i < CardSuit_Count * CardFace_Count; ++i) {
            SampleDeck[i].face_value = (Poker_CardFace)((i % CardFace_Count) + 2);
            SampleDeck[i].suit = (Poker_CardSuit)(i % CardSuit_Count);
            SampleDeck[i].state = CardState_Hidden;
    }
    srand(time(NULL));
}

void
Poker_StartNewRound(Poker_Game* game_state)
{
    deck_index = 0;

    for (int i = 0; i < DECK_SIZE; ++i) {
        DealersDeck[i] = SampleDeck[i];
    }

    for (int shuffles = 0; shuffles < 5; ++shuffles) {
        // Fisher-Yates shuffle
        for (int i = 51; i > 1; --i) {
            int j = rand() % i;
            Poker_Card temp = DealersDeck[i];
            DealersDeck[i] = DealersDeck[j];
            DealersDeck[j] = temp;
        }
    }

    for (int i = 0; i < 2; ++i) {
        game_state->player_hand[i].state = CardState_Hidden;
        game_state->dealer_hand[i].state = CardState_Hidden;
    }

    for (int i = 0; i < 5; ++i) {
        game_state->house_hand[i].state = CardState_Hidden;
    }
}

void
Poker_ProcessNewState(Poker_Game* game_state) {
    switch (game_state->poker_state) {
        case PokerState_PlayerCardsDealt:
        case PokerState_FlopCardsDealt:
        case PokerState_RiverCardsDealt:
        case PokerState_TurnCardsDealt:
            // TODO: Process hand type
            break;
        default:
            break;
    }
}

void
Poker_Update(Poker_Game* game_state) {

}

Poker_CardList* Poker_CreateCardList(Poker_Card first_card) {
    Poker_CardList* list = (Poker_CardList*)malloc(sizeof(Poker_CardList));
    list->first = (Poker_LinkedListNode*)malloc(sizeof(Poker_LinkedListNode));
    list->first->value = first_card;
    list->first->next = NULL;
    list->last = list->first;
    return list;
}

void Poker_AddCardToList(Poker_CardList* card_list, Poker_Card card) {
    card_list->last->next = (Poker_LinkedListNode*)malloc(sizeof(Poker_LinkedListNode));
    card_list->last = card_list->last->next;
    card_list->last->value = card;
    card_list->last->next = NULL;
}

void Poker_DestroyCardList(Poker_CardList* card_list) {
    while(card_list->first) {
        Poker_LinkedListNode* p_node = card_list->first;
        card_list->first = card_list->first->next;
        free(p_node);
    }

    free(card_list);
}
