#ifndef POKER_H_
#define POKER_H_

#include "poker_rank.h"

uint8_t pkrPlayRound(uint8_t playerCount, PkrPlayer *pPlayers);
uint8_t pkrPlayGame(uint8_t playerCount, PkrDecision *pDecisions);
uint8_t pkrDeclareWinner(PkrGame *pGame);

#endif  // POKER_H_
