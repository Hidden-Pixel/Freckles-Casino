#include <gtest/gtest.h>
#include <FC/poker.h>

TEST(Poker_Tests, Linked_List_Test) {
    Poker_Card card1 { .suit = CardSuit_Club, .face_value = CardFace_Ace, .state = CardState_Shown };
    Poker_CardList* list = Poker_CreateCardList(card1);

    ASSERT_NE(nullptr, list);
    ASSERT_EQ(CardFace_Ace, list->first->value.face_value);

    Poker_Card card2 { .suit = CardSuit_Spade, .face_value = CardFace_Ace, .state = CardState_Shown };
    Poker_AddCardToList(list, card2);

    ASSERT_NE(nullptr, list->first->next);
    ASSERT_EQ(CardSuit_Spade, list->first->next->value.suit);

    Poker_Card card3 { .suit = CardSuit_Heart, .face_value = CardFace_Ace, .state = CardState_Shown };
    Poker_AddCardToList(list, card3);

    ASSERT_NE(nullptr, list->last);
    ASSERT_EQ(CardSuit_Heart, list->last->value.suit);

    ASSERT_EQ(nullptr, list->last->next);

    Poker_DestroyCardList(list);
}

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

    auto best_hand = Poker_FindAllHands(player_cards, nullptr, 0);

    ASSERT_EQ(PokerHand_Pair, best_hand);
}
