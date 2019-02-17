// Callbacks for input

#pragma once

#include <FC/poker.h>

typedef void
(*Func_CardHold_Pressed)(Poker_Card* /* card_hand */, int /* card_index */, unsigned char /* cursor_state */);
typedef void
(*Func_CardHold_Complete)(Poker_Card* /* card_hand */, Poker_CardState /* card_visibility */, int /* hand_size */);
typedef void
(*Func_GameOver)(Poker_Hand /* dealer_hand */, Poker_Hand /* player_hand */);

Func_CardHold_Pressed Command_OnCardHoldPressed;
Func_CardHold_Complete Command_OnCardHoldComplete;
Func_GameOver Command_OnGameOver;
