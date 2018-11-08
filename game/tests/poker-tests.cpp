#include <gtest/gtest.h>
#include <FC/poker.h>

TEST(Poker_Tests, Finds_Best_Hand_With_Only_Player_Cards) {
    Poker_Card player_cards[2];
    Poker_Card house_cards[5];

    player_cards[0] = Poker_Card {
        .suit = CardSuit_Diamond,
        .face_value = CardFace_Ace,
        .state = CardState_Shown
    };

    player_cards[1] = Poker_Card {
        .suit = CardSuit_Club,
        .face_value = CardFace_Ace,
        .state = CardState_Shown
    };

    auto best_hand = Poker_FindBestHand(player_cards, nullptr, 0);

    ASSERT_EQ(PokerHand_Pair, best_hand);
}
