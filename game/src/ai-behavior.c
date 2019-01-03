#include <FC/ai-behavior.h>

Poker_RankedHand_5
AI_FiveCardDraw_RankHand(Poker_Card* hand) {
    Poker_Card sorted_hand[5];
    for (int i = 0; i < 5; ++i) {
        sorted_hand[i] = hand[i];
    }

    // NOTE: Insertion sort, it's 5 freaking cards :-D
    for (int i = 0; i < 4; ++i) {
        for (int j = i+1; j < 5; ++j) {
            if (sorted_hand[i].face_value < sorted_hand[j].face_value) {
                Poker_Card temp = sorted_hand[j];
                sorted_hand[j] = sorted_hand[i];
                sorted_hand[i] = temp;
                j = i+1;
            }
        }
    }

    int card_counts[CardFace_Count] = { 0 };
    for (int i = 0; i < 5; ++i) {
        card_counts[sorted_hand[i].face_value]++;
    }

    Poker_RankedHand_5 result;
    result.hand_type = Poker_FindBestHand(sorted_hand, 5);
    result.card_1 = sorted_hand[0];
    result.card_2 = sorted_hand[1];
    result.card_3 = sorted_hand[2];
    result.card_4 = sorted_hand[3];
    result.card_5 = sorted_hand[4];

    return result;
}
