// Callbacks for input

#pragma once

typedef void
(*Func_CardHold_Pressed)(Poker_Card* /* card_hand */, int /* card_index */, unsigned char /* cursor_state */);
typedef void
(*Func_CardHold_Complete)(Poker_Card* /* card_hand */, int /* hand_size */);

global_variable Func_CardHold_Pressed Command_OnCardHoldPressed = 0;
global_variable Func_CardHold_Complete Command_OnCardHoldComplete = 0;

