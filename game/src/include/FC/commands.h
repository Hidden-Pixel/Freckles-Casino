// Callbacks for input

#pragma once

#include <FC/poker.h>

typedef void
(*Func_CardHold_Pressed)(Poker_Card* /* card_hand */, int /* card_index */, unsigned char /* cursor_state */);
typedef void
(*Func_CardHold_Complete)(Poker_Card* /* card_hand */, Poker_CardState /* card_visibility */, int /* hand_size */);

extern Func_CardHold_Pressed Command_OnCardHoldPressed;
extern Func_CardHold_Complete Command_OnCardHoldComplete;


