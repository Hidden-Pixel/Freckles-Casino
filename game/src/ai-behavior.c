#include <FC/ai-behavior.h>

Poker_RankedHand_5
AI_FiveCardDraw_RankHand(const Poker_Card* hand) {
    Poker_Card sorted_hand[5];
    for (int i = 0; i < 5; ++i) {
        sorted_hand[i] = hand[i];
    }

    // NOTE: Insertion sort, it's 5 freaking cards :-D
    for (int i = 0; i < 4; ++i) {
        for (int j = i+1; j < 5; ++j) {
            if ((sorted_hand[i].face_value < sorted_hand[j].face_value) ||
                (sorted_hand[i].face_value == sorted_hand[j].face_value &&
                   Poker_CardRank(sorted_hand[i]) < Poker_CardRank(sorted_hand[j]))) {
                Poker_Card temp = sorted_hand[j];
                sorted_hand[j] = sorted_hand[i];
                sorted_hand[i] = temp;
                j = i+1;
            }
        }
    }

    Poker_RankedHand_5 result = { .hand_type = PokerHand_None };
    for (int i = 0; i < 5; ++i) {
        result.card_counts[sorted_hand[i].face_value]++;
    }
    Poker_ScoredHand_5 scoredHand = Poker_ScoreHand_5(sorted_hand);
    result.hand_type = scoredHand.hand_type;
    result.ranked_cards[0] = sorted_hand[0];
    result.ranked_cards[1] = sorted_hand[1];
    result.ranked_cards[2] = sorted_hand[2];
    result.ranked_cards[3] = sorted_hand[3];
    result.ranked_cards[4] = sorted_hand[4];

    return result;
}


void
AI_FiveCardDraw_MakeHoldDecision(Poker_Card* hand) {
    Poker_RankedHand_5 rankedHand = AI_FiveCardDraw_RankHand(hand);
    int rankedToHandMap[5];

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (Poker_CardRank(rankedHand.ranked_cards[i]) == Poker_CardRank(hand[j])) {
                rankedToHandMap[i] = j;
            }
        }
    }

    switch(rankedHand.hand_type) {
        case PokerHand_Pair:
        case PokerHand_FourOfAKind: {
            for (int i = 0; i < 5; ++i) {
                if (rankedHand.card_counts[rankedHand.ranked_cards[i].face_value] > 1) {
                    rankedHand.ranked_cards[i].hold = HoldState_Held;
                } else {
                    rankedHand.ranked_cards[i].hold = HoldState_NotHeld;
                }
            }
        } break;
        case PokerHand_ThreeOfAKind:
        case PokerHand_TwoPair: {
            int held = 0;
            for (int i = 0; i < 5; ++i) {
                if (rankedHand.card_counts[rankedHand.ranked_cards[i].face_value] > 1) {
                    rankedHand.ranked_cards[i].hold = HoldState_Held;
                } else {
                    // Only hold the highest card
                    if (!held) {
                        rankedHand.ranked_cards[i].hold = HoldState_Held;
                        held = 1;
                    } else {
                        rankedHand.ranked_cards[i].hold = HoldState_NotHeld;
                    }
                }
            }
        } break;
        case PokerHand_HighCard: {
            rankedHand.ranked_cards[0].hold = HoldState_Held;
            rankedHand.ranked_cards[1].hold = HoldState_NotHeld;
            rankedHand.ranked_cards[2].hold = HoldState_NotHeld;
            rankedHand.ranked_cards[3].hold = HoldState_NotHeld;
            rankedHand.ranked_cards[4].hold = HoldState_NotHeld;
        } break;

        // Easy street: handling hands that take all five cards
        default:
            rankedHand.ranked_cards[0].hold = HoldState_Held;
            rankedHand.ranked_cards[1].hold = HoldState_Held;
            rankedHand.ranked_cards[2].hold = HoldState_Held;
            rankedHand.ranked_cards[3].hold = HoldState_Held;
            rankedHand.ranked_cards[4].hold = HoldState_Held;
            break;
    }

    hand[0] = rankedHand.ranked_cards[rankedToHandMap[0]];
    hand[1] = rankedHand.ranked_cards[rankedToHandMap[1]];
    hand[2] = rankedHand.ranked_cards[rankedToHandMap[2]];
    hand[3] = rankedHand.ranked_cards[rankedToHandMap[3]];
    hand[4] = rankedHand.ranked_cards[rankedToHandMap[4]];
}
