#pragma once

#include <FC/poker.h>

freckles::poker::RankedHand AI_FiveCardDraw_RankHand(const std::vector<freckles::poker::Card>& hand);

// TODO(Alex): Is this easy difficulty?
void AI_FiveCardDraw_MakeHoldDecision(std::vector<freckles::poker::Card>& hand);
