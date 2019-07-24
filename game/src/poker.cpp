#include <FC/poker.h>
#include <FC/buffer.h>

#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <memory.h>

using namespace freckles;

// There's 52 cards in a deck and we're NOT counting jokers, folks.
const int DeckSize = 52;
const int MaxHandCombos = 22;
const int MaxHolds = 5;
const int DefaultAnte = 25
const int StraightHands = 10;

const char* CardSuit_Names[poker::CardSuitCount] =
        {
                "C", "D", "H", "S"
        };

const char* CardFace_ShortNames[poker::CardFaceCount] =
        {
                "2", "3", "4", "5",
                "6", "7", "8", "9",
                "10", "J", "Q", "K",
                "A"
        };

const char* CardFace_FullNames[poker::CardFaceCount] =
        {
                "Two", "Three", "Four", "Five",
                "Six", "Seven", "Eight", "Nine",
                "Ten", "Jack", "Queen", "King",
                "Ace"
        };

const char* Hand_Names[poker::PossibleHandCount] =
        {
                "High Card",
                "Pair",
                "Two Pair",
                "Three of a Kind",
                "Straight",
                "Flush",
                "Full House",
                "Four of a Kind",
                "Straight Flush",
                "Royal Flush"
        };

static std::vector<int> Straights(StraightHands);
static std::vector<poker::Card> DealersDeck(DeckSize);
static std::vector<poker::Card> SampleDeck(DeckSize);
static int deck_index = 0;

void freckles::poker::cache_possible_hands()
{
    // Create all values for hands with a straight in them
    for (int i = 0; i < StraightHands-1; ++i) {
        // NOTE: Assumes little endian
        int straight = 0;
        for (int j = i; j < i + 5; ++j) {
            straight |= (1 << j);
        }
        Straights[i] = straight;
    }

    // For Ace-2-3-4-5
    int fiveHighStraight = (1u << static_cast<unsigned>(CardFace::Ace)) | (1u << static_cast<unsigned>(CardFace::Two))
            | (1u << static_cast<unsigned>(CardFace::Three)) | (1u << static_cast<unsigned>(CardFace::Four)) | (1u << static_cast<unsigned>(CardFace::Five));
    Straights[StraightHands-1] = fiveHighStraight;
}

void hold_cards(std::vector<poker::Card>& card_hand, poker::CardState card_visibility)
{

}

void freckles::poker::start_five_card_draw(freckles::poker::Game& game_state){
    for (int i = 0; i < DeckSize; ++i)
    {
        SampleDeck[i].suit = static_cast<poker::CardSuit>(i % poker::CardSuitCount);
        SampleDeck[i].face_value = static_cast<poker::CardFace>(i % poker::CardFaceCount);
        SampleDeck[i].state = poker::CardState::Hidden;
    }

    game_state.poker_type = poker::GameType::FiveCard;
    game_state.poker_state = poker::PokerState::NotStarted;
    game_state.player_hand.clear();
    game_state.dealer_hand.clear();

    for (int i = 0; i < 5; ++i) {
        game_state.player_hand.emplace_back(poker::Card());
        game_state.dealer_hand.emplace_back(poker::Card());
    }

    // player_hand_type
    game_state.player_hand_type = poker::HandResult::None;
    game_state.dealer_hand_type = poker::HandResult::None;
    game_state.player_score = 1000;
    game_state.dealer_score = 1000;
    game_state.betting_round = 0;
    game_state.chances_left = 3;

    srand(time(NULL));
    poker::cache_possible_hands();
    game_state.on_cardhold_pressed = [](poker::Card& card, poker::CardHoldState state) -> {
        card.hold = state;
    };
    game_state.on_cardhold_complete = [](std::vector<poker::Card>& card_hand, poker::CardState card_visibility) -> {
        for (auto& card : card_hand) {
            if (card.hold == poker::CardHoldState::NotHeld) {
                card = poker::draw_one(card_visibility);
            }
        }
    };
}

void
freckles::poker::start_texas_holdem(freckles::poker::Game& game_state){
    game_state.player_hand.clear();
    game_state.dealer_hand.clear();

    for (int i = 0; i < 2; ++i) {
        game_state.player_hand.emplace_back(poker::Card());
        game_state.dealer_hand.emplace_back(poker::Card());
    }

    // TODO: house_hand

    // player_hand_type
    game_state.player_hand_type = poker::HandResult::None;
    game_state.dealer_hand_type = poker::HandResult::None;
    game_state.chances_left = 0;
    game_state.player_score = 0;
    game_state.dealer_score = 0;
}

int freckles::poker::rank_card(const freckles::poker::Card& card){
    return card.suit * 13 + card.face_value;
}

poker::HandResult freckles::poker::find_best_hand(const std::vector<freckles::poker::Card>& player_hand) {
    std::vector<int> card_counts(CardFaceCount);
    std::vector<int> suit_counts(CardSuitCount);
    std::vector<int> hand_flags(PossibleHandCount);
    int hand_bits = 0;

    for (int i = 0; i < player_hand.size(); ++i) {
        card_counts[player_hand[i].face_value] += 1;
        suit_counts[player_hand[i].suit] += 1;
        // 13 bit flags for cards in hand.
        // 0 0000 0001 1111 would be a low straight.
        hand_bits |= (1u << static_cast<unsigned>(player_hand[i].face_value));
    }

    for (int i = 0; i < CardSuitCount; ++i) {
        if (suit_counts[i] == 5) {
            hand_flags[static_cast<int>(poker::HandResult::Flush)] += 1;
            break;
        }
    }

    for (int i = 0; i < StraightHands; ++i) {
        if (Straights[i] == hand_bits) {
            hand_flags[static_cast<int>(poker::HandResult::Straight)] += 1;
            break;
        }
    }

    // Figure out if we have a straight flush or royal flush
    if (hand_flags[static_cast<int>(poker::HandResult::Straight)] > 0 && hand_flags[static_cast<int>(poker::HandResult::Flush)] > 0) {
        if (card_counts[static_cast<int>(poker::CardFace::Ace)] > 0) {
            return poker::HandResult::RoyalFlush;
        }

        return poker::HandResult::StraightFlush;
    }

    if (hand_flags[static_cast<int>(poker::HandResult::Flush)] > 0) {
        return poker::HandResult::Flush;
    }

    if (hand_flags[static_cast<int>(poker::HandResult::Straight)] > 0) {
        return poker::HandResult::Straight;
    }

    for (int i = 0; i < poker::CardFaceCount; ++i) {
        if (card_counts[i] == 2) {
            hand_flags[static_cast<int>(poker::HandResult::Pair)] += 1;
        }
        if (card_counts[i] == 3) {
            hand_flags[static_cast<int>(poker::HandResult::ThreeOfAKind)] += 1;
        }
        if (card_counts[i] == 4) {
            hand_flags[static_cast<int>(poker::HandResult::FourOfAKind)] += 1;
        }
    }

    if (hand_flags[static_cast<int>(poker::HandResult::Pair)] > 0 && hand_flags[static_cast<int>(poker::HandResult::ThreeOfAKind)] > 0) {
        return poker::HandResult::FullHouse;
    }

    if (hand_flags[static_cast<int>(poker::HandResult::FourOfAKind)] > 0) {
        return poker::HandResult::FourOfAKind;
    }

    if (hand_flags[static_cast<int>(poker::HandResult::ThreeOfAKind)] > 0) {
        return poker::HandResult::ThreeOfAKind;
    }

    if (hand_flags[static_cast<int>(poker::HandResult::Pair)] == 2) {
        return poker::HandResult::TwoPair;
    }

    if (hand_flags[static_cast<int>(poker::HandResult::Pair)] > 0) {
        return poker::HandResult::Pair;
    }

    return poker::HandResult::HighCard;
}

poker::Card freckles::poker::draw_one(freckles::poker::CardState state){
    assert(deck_index < DeckSize);
    poker::Card card = DealersDeck[deck_index];
    card.state = state;
    deck_index++;
    return card;
}

void freckles::poker::shuffle_deck(std::vector<freckles::poker::Card>& deck){
    deck_index = 0;
    for (int i = 0; i < DeckSize; ++i)
    {
        deck[i] = SampleDeck[i];
    }
    for (int shuffles = 0; shuffles < 5; ++shuffles) 
    {
        // Fisher-Yates shuffle
        for (int i = 51; i > 1; --i) 
        {
            int j = rand() % i;
            poker::Card temp = DealersDeck[i];
            DealersDeck[i] = DealersDeck[j];
            DealersDeck[j] = temp;
        }
    }
}

void freckles::poker::deal(freckles::poker::Game& game_state, int hand_size){
    int i = 0;
    switch (game_state.poker_type)
    {
        case poker::GameType::FiveCard:
        {
            i = 5;
        } break;

        case poker::GameType::Holdem:
        {
            i = 2;
        } break;
    }
    while (i >= 0)
    {
        game_state.player_hand[i] = poker::draw_one(poker::CardState::Shown);
        game_state.dealer_hand[i] = poker::draw_one(poker::CardState::Hidden);
        i--;
    }
    
    game_state.poker_state = poker::PokerState::PlayerCardsDealt;
}

void freckles::poker::reveal_hand(std::vector<freckles::poker::Card>& hand) {
    for (auto& card : hand) {
        card.state = poker::CardState::Shown;
    }
}

void freckles::poker::start_new_round(freckles::poker::Game& game_state) {
    game_state->poker_state = poker::PokerState::Started;
    poker::shuffle_deck(DealersDeck);
    poker::deal(game_state);
    game_state.betting_round = 0;
}

void freckles::poker::update(freckles::poker::Game& game_state) {
    switch (game_state.poker_state)
    {
        case poker::PokerState::Started:
        {
            if (game_state->player_score > DefaultAnte)
            {
                poker::start_new_round(game_state);
                game_state.player_score -= DefaultAnte;
            } 
            else
            {
                game_state.poker_state = poker::PokerState::GameOver;
            }
        } break;

        case poker::PokerState::PlayerCardsDealt:
        {
            game_state.poker_state = poker::PokerState::Betting;
            game_state.betting_round++;
        } break;

        case poker::PokerState::SelectHolds:
        {
            // TODO(nick): ...
        } break;

        case poker::PokerState::Betting:
        {
            // TODO(nick): start betting process ...
            if (game_state.betting_round == 1)
            {
                // TODO(nick):
                game_state.poker_state = poker::PokerState::SelectHolds;
            }
            else
            {
                // TODO(nick):
            }
        } break;

        case poker::PokerState::ExchangeCards:
        {
            // TODO(nick): only allow one betting round ...
        } break;

        case poker::PokerState::GameOver:
        {

        } break;
    }
}