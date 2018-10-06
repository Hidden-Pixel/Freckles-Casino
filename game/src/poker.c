#include <FC/poker.h>
#include <FC/defines.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define DECK_SIZE 52

local_variable Poker_Card DealersDeck[DECK_SIZE];
local_variable Poker_Card SampleDeck[DECK_SIZE];
local_variable int deck_index = 0;


Poker_Card
Poker_DrawOne(){
    assert(deck_index < DECK_SIZE);
    Poker_Card card = DealersDeck[deck_index];
    deck_index++;
    return card;
}

void
Poker_Init()
{
    for (int i = 0; i < Poker_SuitCount * Poker_FaceCount; ++i) {
            SampleDeck[i].face_value = (i % Poker_FaceCount) + 2;
            SampleDeck[i].suit = i % Poker_SuitCount;
    }
    srand(time(NULL));
}

void
Poker_StartNewRound()
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
}
