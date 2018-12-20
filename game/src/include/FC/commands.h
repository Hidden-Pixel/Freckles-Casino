// Callbacks for input

#pragma once

typedef void
(*Func_CardHold_Pressed)(Poker_Card* /* card_hand */, int /* card_index */, unsigned char /* cursor_state */);

global_variable Func_CardHold_Pressed Command_OnCardHoldPressed = 0;
