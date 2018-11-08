#include <FC/poker.h>
#include <FC/defines.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <tgmath.h>

// There's 52 cards in a deck and we're NOT counting jokers, folks.
#define DECK_SIZE 52

local_variable Poker_Card DealersDeck[DECK_SIZE] = { };
local_variable Poker_Card SampleDeck[DECK_SIZE] = { };
local_variable int deck_index = 0;

/*
 * @remark We're assuming that player_hand is length 2
 */
Poker_Hand
Poker_FindBestHand(Poker_Card* player_hand, Poker_Card* house_cards, int house_card_count) {
    // Create all the possible hands
    int maxCombinations = pow(2, house_card_count);
    Poker_Card hands[maxCombinations][maxCombinations];
    for (int i = 0; i < maxCombinations; ++i) {

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
