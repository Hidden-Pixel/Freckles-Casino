#include <FC/poker.h>
#include <FC/defines.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define DECK_SIZE 52

local_persist Poker_Card DealersDeck[DECK_SIZE] = { 0 };
local_persist Poker_Card SampleDeck[DECK_SIZE] = { 0 };
local_persist int deck_index = 0;

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
            SampleDeck[i].face_value = (i % CardFace_Count) + 2;
            SampleDeck[i].suit = i % CardSuit_Count;
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
