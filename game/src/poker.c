#include <FC/poker.h>
#include <FC/defines.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

// There's 52 cards in a deck and we're NOT counting jokers, folks.
#define DECK_SIZE 52

local_persist Poker_Card DealersDeck[DECK_SIZE] = { 0 };
local_persist Poker_Card SampleDeck[DECK_SIZE] = { 0 };
local_persist int deck_index = 0;

/*
 * @remark We're assuming that player_hand is length 2
 */
Poker_Hand
Poker_FindBestHand(Poker_Card* player_hand, Poker_Card* house_cards, int house_card_count) {
    int card_count_squared = pow(2, house_card_count);

    // combinations: f(x) = h(h-x), where h is the house count and x is the size of picks
    int two_combinations = house_card_count > 3 ? card_count_squared - house_card_count * 3 : 1;

    // We'll pre-calculate these, at least until we decide we want to support Omaha
    int one_combinations = house_card_count > 4 ? 5 : 0;
    int zero_combinations = house_card_count > 4 ? 1 : 0;
    int max_combinations = two_combinations + one_combinations + zero_combinations;

    // Create all the possible hands
    // TODO(nick): add link link struct and functions
    // variable length arrays are not supported by all c / c++ compilers :(
    // https://msdn.microsoft.com/en-us/library/zb1574zs.aspx
    //Poker_Card hands[max_combinations][house_card_count + 2];
    Poker_Card hands[100][100];

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
