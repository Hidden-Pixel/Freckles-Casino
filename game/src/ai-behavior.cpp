#include <FC/ai-behavior.h>
#include <algorithm>
using namespace freckles;

poker::RankedHand AI_FiveCardDraw_RankHand(const std::vector<poker::Card>& hand) {
    std::vector<poker::Card> sorted_hand(hand.begin(), hand.end());
    std::sort(sorted_hand.begin(), sorted_hand.end(), [](const poker::Card& a, const poker::Card& b) {
        return a.face_value < b.face_value ||
                (a.face_value == b.face_value && poker::rank_card(a) < poker::rank_card(b));
    });

    poker::RankedHand result;
    result.hand_type = poker::HandResult::None;
    for (int i = 0; i < 5; ++i) {
        result.card_counts[static_cast<int>(sorted_hand[i].face_value)]++;
    }
    result.hand_type = poker::find_best_hand(sorted_hand);
    result.ranked_cards[0] = sorted_hand[0];
    result.ranked_cards[1] = sorted_hand[1];
    result.ranked_cards[2] = sorted_hand[2];
    result.ranked_cards[3] = sorted_hand[3];
    result.ranked_cards[4] = sorted_hand[4];

    return result;
}


void AI_FiveCardDraw_MakeHoldDecision(std::vector<poker::Card>& hand) {
    poker::RankedHand rankedHand = AI_FiveCardDraw_RankHand(hand);
    int rankedToHandMap[5];

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (poker::rank_card(rankedHand.ranked_cards[i]) == poker::rank_card(hand[j])) {
                rankedToHandMap[i] = j;
            }
        }
    }

    switch(rankedHand.hand_type) {
        case poker::HandResult::Pair:
        case poker::HandResult::FourOfAKind: {
            for (int i = 0; i < 5; ++i) {
                if (rankedHand.card_counts[static_cast<int>(rankedHand.ranked_cards[i].face_value)] > 1) {
                    rankedHand.ranked_cards[i].hold = poker::CardHoldState::Held;
                } else {
                    rankedHand.ranked_cards[i].hold = poker::CardHoldState::NotHeld;
                }
            }
        } break;
        case poker::HandResult::ThreeOfAKind:
        case poker::HandResult::TwoPair: {
            int held = 0;
            for (int i = 0; i < 5; ++i) {
                if (rankedHand.card_counts[static_cast<int>(rankedHand.ranked_cards[i].face_value)] > 1) {
                    rankedHand.ranked_cards[i].hold = poker::CardHoldState::Held;
                } else {
                    // Only hold the highest card
                    if (!held) {
                        rankedHand.ranked_cards[i].hold = poker::CardHoldState::Held;
                        held = 1;
                    } else {
                        rankedHand.ranked_cards[i].hold = poker::CardHoldState::NotHeld;
                    }
                }
            }
        } break;
        case poker::HandResult::HighCard: {
            rankedHand.ranked_cards[0].hold = poker::CardHoldState::Held;
            rankedHand.ranked_cards[1].hold = poker::CardHoldState::NotHeld;
            rankedHand.ranked_cards[2].hold = poker::CardHoldState::NotHeld;
            rankedHand.ranked_cards[3].hold = poker::CardHoldState::NotHeld;
            rankedHand.ranked_cards[4].hold = poker::CardHoldState::NotHeld;
        } break;

        // Easy street: handling hands that take all five cards
        default:
            rankedHand.ranked_cards[0].hold = poker::CardHoldState::Held;
            rankedHand.ranked_cards[1].hold = poker::CardHoldState::Held;
            rankedHand.ranked_cards[2].hold = poker::CardHoldState::Held;
            rankedHand.ranked_cards[3].hold = poker::CardHoldState::Held;
            rankedHand.ranked_cards[4].hold = poker::CardHoldState::Held;
            break;
    }

    hand[0] = rankedHand.ranked_cards[rankedToHandMap[0]];
    hand[1] = rankedHand.ranked_cards[rankedToHandMap[1]];
    hand[2] = rankedHand.ranked_cards[rankedToHandMap[2]];
    hand[3] = rankedHand.ranked_cards[rankedToHandMap[3]];
    hand[4] = rankedHand.ranked_cards[rankedToHandMap[4]];
}
