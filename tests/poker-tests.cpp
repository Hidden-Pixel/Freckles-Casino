#include <FC/poker.h>
#include <FC/commands.h>
#include <FC/input.h>

#undef internal

#include <gtest/gtest.h>

TEST(Poker_Tests, Callback_Test) {
    Poker_Card hand[1];
    Poker_Card card {
        .suit = CardSuit_Spade,
        .face_value = CardFace_Eight,
        .state = CardState_Shown,
        .hold = CURSOR_NONE
    };

    hand[0] = card;

    Command_OnCardHoldPressed = [](Poker_Card* card_hand, int card_index, unsigned char hold_state) {
        card_hand[card_index].hold = hold_state;
    };
    Command_OnCardHoldPressed(hand, 0, CURSOR_SELECTED);
    ASSERT_EQ(CURSOR_SELECTED, hand[0].hold);
}

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

TEST(Poker_Tests, Finds_Flush) {
    Poker_Card player_cards[5];

    player_cards[0] = Poker_Card {
        .suit = CardSuit_Club,
        .face_value = CardFace_Ace,
        .state = CardState_Shown
    };

    player_cards[1] = Poker_Card {
        .suit = CardSuit_Club,
        .face_value = CardFace_Six,
        .state = CardState_Shown
    };

    player_cards[2] = Poker_Card {
        .suit = CardSuit_Club,
        .face_value = CardFace_King,
        .state = CardState_Shown
    };

    player_cards[3] = Poker_Card {
        .suit = CardSuit_Club,
        .face_value = CardFace_Eight,
        .state = CardState_Shown
    };

    player_cards[4] = Poker_Card {
            .suit = CardSuit_Club,
            .face_value = CardFace_Four,
            .state = CardState_Shown
    };

    auto best_hand = Poker_FindBestHand(player_cards, 5);

    ASSERT_EQ(PokerHand_Flush, best_hand);
}

TEST(Poker_Tests, Finds_Straight) {
    Poker_CacheHands();
    Poker_Card player_cards[5];

    player_cards[0] = Poker_Card {
            .suit = CardSuit_Club,
            .face_value = CardFace_Two,
            .state = CardState_Shown
    };

    player_cards[1] = Poker_Card {
            .suit = CardSuit_Club,
            .face_value = CardFace_Three,
            .state = CardState_Shown
    };

    player_cards[2] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Four,
            .state = CardState_Shown
    };

    player_cards[3] = Poker_Card {
            .suit = CardSuit_Club,
            .face_value = CardFace_Five,
            .state = CardState_Shown
    };

    player_cards[4] = Poker_Card {
            .suit = CardSuit_Heart,
            .face_value = CardFace_Six,
            .state = CardState_Shown
    };

    auto best_hand = Poker_FindBestHand(player_cards, 5);

    ASSERT_EQ(PokerHand_Straight, best_hand);
}

TEST(Poker_Tests, Finds_Royal_Flush) {
    Poker_CacheHands();
    Poker_Card player_cards[5];

    player_cards[0] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[1] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Jack,
            .state = CardState_Shown
    };

    player_cards[2] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Queen,
            .state = CardState_Shown
    };

    player_cards[3] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_King,
            .state = CardState_Shown
    };

    player_cards[4] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Ace,
            .state = CardState_Shown
    };

    auto best_hand = Poker_FindBestHand(player_cards, 5);

    ASSERT_EQ(PokerHand_RoyalFlush, best_hand);
}

TEST(Poker_Tests, Finds_Two_Pair) {
    Poker_CacheHands();
    Poker_Card player_cards[5];

    player_cards[0] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[1] = Poker_Card {
            .suit = CardSuit_Heart,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[2] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Queen,
            .state = CardState_Shown
    };

    player_cards[3] = Poker_Card {
            .suit = CardSuit_Diamond,
            .face_value = CardFace_Queen,
            .state = CardState_Shown
    };

    player_cards[4] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Ace,
            .state = CardState_Shown
    };

    auto best_hand = Poker_FindBestHand(player_cards, 5);

    ASSERT_EQ(PokerHand_TwoPair, best_hand);
}

TEST(Poker_Tests, Finds_Three_of_a_kind) {
    Poker_CacheHands();
    Poker_Card player_cards[5];

    player_cards[0] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[1] = Poker_Card {
            .suit = CardSuit_Heart,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[2] = Poker_Card {
            .suit = CardSuit_Diamond,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[3] = Poker_Card {
            .suit = CardSuit_Diamond,
            .face_value = CardFace_Queen,
            .state = CardState_Shown
    };

    player_cards[4] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Ace,
            .state = CardState_Shown
    };

    auto best_hand = Poker_FindBestHand(player_cards, 5);

    ASSERT_EQ(PokerHand_ThreeOfAKind, best_hand);
}

TEST(Poker_Tests, Finds_Full_house) {
    Poker_CacheHands();
    Poker_Card player_cards[5];

    player_cards[0] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[1] = Poker_Card {
            .suit = CardSuit_Heart,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[2] = Poker_Card {
            .suit = CardSuit_Diamond,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[3] = Poker_Card {
            .suit = CardSuit_Diamond,
            .face_value = CardFace_Queen,
            .state = CardState_Shown
    };

    player_cards[4] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Queen,
            .state = CardState_Shown
    };

    auto best_hand = Poker_FindBestHand(player_cards, 5);

    ASSERT_EQ(PokerHand_FullHouse, best_hand);
}

TEST(Poker_Tests, Finds_Four_of_a_kind) {
    Poker_CacheHands();
    Poker_Card player_cards[5];

    player_cards[0] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[1] = Poker_Card {
            .suit = CardSuit_Heart,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[2] = Poker_Card {
            .suit = CardSuit_Diamond,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[3] = Poker_Card {
            .suit = CardSuit_Club,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[4] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Queen,
            .state = CardState_Shown
    };

    auto best_hand = Poker_FindBestHand(player_cards, 5);

    ASSERT_EQ(PokerHand_FourOfAKind, best_hand);
}

TEST(Poker_Tests, Finds_High_card) {
    Poker_CacheHands();
    Poker_Card player_cards[5];

    player_cards[0] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_Ten,
            .state = CardState_Shown
    };

    player_cards[1] = Poker_Card {
            .suit = CardSuit_Heart,
            .face_value = CardFace_Four,
            .state = CardState_Shown
    };

    player_cards[2] = Poker_Card {
            .suit = CardSuit_Diamond,
            .face_value = CardFace_Six,
            .state = CardState_Shown
    };

    player_cards[3] = Poker_Card {
            .suit = CardSuit_Diamond,
            .face_value = CardFace_Queen,
            .state = CardState_Shown
    };

    player_cards[4] = Poker_Card {
            .suit = CardSuit_Spade,
            .face_value = CardFace_King,
            .state = CardState_Shown
    };

    auto best_hand = Poker_FindBestHand(player_cards, 5);

    ASSERT_EQ(PokerHand_HighCard, best_hand);
}
