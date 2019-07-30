#pragma once

#include <FC/defines.h>
#include <vector>

namespace freckles {
    namespace poker {

        const int CardFaceCount = 13;
        const int CardSuitCount = 4;
        const int PossibleHandCount = 10;

        extern const char* CardSuit_Names[CardSuitCount];
        extern const char* CardFace_ShortNames[CardFaceCount];
        extern const char* CardFace_FullNames[CardFaceCount];
        extern const char* Hand_Names[PossibleHandCount];

        enum class CardSuit {
            None = -1,
            Club = 0,
            Diamond = 1,
            Heart = 2,
            Spade = 3,
        };

        enum class CardFace {
            None = -1,
            Two = 0,
            Three = 1,
            Four = 2,
            Five = 3,
            Six = 4,
            Seven = 5,
            Eight = 6,
            Nine = 7,
            Ten = 8,
            Jack = 9,
            Queen = 10,
            King = 11,
            Ace = 12,
        };

        enum class GameType {
            None = 0,
            FiveCard = 1,
            Holdem = 2,
        };

        enum class CardState {
            None = 0,
            Hidden = 1,
            Shown = 2,
        };

        enum class PokerState {
            NotStarted = 0,
            Started = 1,
            Shuffled = 2,
            PlayerCardsDealt = 3,
            FlopCardsDealt = 4,
            RiverCardsDealt = 5,
            TurnCardsDealt = 6,
            SelectHolds = 7,
            Betting = 8,
            GameOver = 9,
            ExchangeCards = 10,
        };

        enum class HandResult {
            None = -1,
            HighCard = 0,
            Pair = 1,
            TwoPair = 2,
            ThreeOfAKind = 3,
            Straight = 4,
            Flush = 5,
            FullHouse = 6,
            FourOfAKind = 7,
            StraightFlush = 8,
            RoyalFlush = 9
        };

        enum class CardHoldState {
            NotHeld = 0,
            Held = 2
        };

        struct Card {
            Card(CardSuit suit = CardSuit::None,
                    CardFace face_value = CardFace::None,
                    CardState state = CardState::None,
                    CardHoldState hold_state = CardHoldState::NotHeld)
                    : suit(suit),
                        face_value(face_value),
                        state(state),
                        hold(hold_state),
                        selected(false) {
            }

            CardSuit suit;
            CardFace face_value;
            CardState state;
            CardHoldState hold;
            bool selected;
        };

        template <unsigned int HandSize>
        struct RankedHand {
            RankedHand() :
                hand_type(HandResult::None),
                card_counts(std::vector<int>(52)),
                ranked_cards(std::vector<Card>(HandSize)) {

            }

            HandResult hand_type;
            std::vector<int> card_counts;
            // 0 is the highest ranked card.
            std::vector<Card> ranked_cards;
        };

        struct Game {
            PokerState poker_state;
            GameType poker_type;
            std::vector<Card> player_hand;
            std::vector<Card> dealer_hand;
            HandResult player_hand_type;
            HandResult dealer_hand_type;
            int betting_round;
            int chances_left;
            int player_score;
            int dealer_score;

            std::function<void(Card& /* old_card */, Card& /* new_card */)> on_cursor_change;
            std::function<void(Card& /* card */)> on_cardhold_pressed;
            std::function<void(std::vector<Card>& /* card_hand */, CardState /* card_visibility */)> on_cardhold_complete;
            std::function<void(HandResult /* dealer_hand */, HandResult /* player_hand */)> on_game_over;
            std::function<void(PokerState /* old_state */, PokerState /* new_state */)> on_state_change;
        };

        void change_poker_state(Game& game, PokerState new_state);

        void start_five_card_draw(Game& game_state);

        void start_texas_holdem(Game& game_state);

        void start_new_round(Game& game_state);

        void process_game_state(Game& game_state);

        void update(Game& game_state);

        HandResult find_all_hand_results(std::vector<Card> player_hand, std::vector<Card> house_cards,
                int house_card_count);

        void deal(Game& game_state, int hand_size);

        Card draw_one(CardState state);

        void shuffle_deck(std::vector<Card>& deck);

        void reveal_hand(std::vector<Card>& hand);

        void cache_possible_hands();

        HandResult find_best_hand(const std::vector<Card>& player_hand);

        int rank_card(const Card& card);

        void hold_card(Game& game_state, Card& card_to_hold);

    } // namespace poker
} // namespace freckles
