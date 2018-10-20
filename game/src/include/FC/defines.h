#pragma once

#define local_persist   static
#define global_variable static
#define internal        static

#define len(array)(sizeof(array)/sizeof(array[0]))

void
DrawFaceCard(Poker_Card card, int x, int y);
