#ifndef POKER_PLAYER_H_
#define POKER_PLAYER_H_

#include "poker_card.h"

// This is a callback to the player decision method
typedef uint32_t (*PkrDecision)(void *, const uint8_t);

typedef struct PkrPlayer_T {
	uint8_t folded;
	uint32_t balance;
	uint32_t bet;
	PkrDecision pDecisionCallback;
} PkrPlayer;

typedef struct PkrGame_T {
	PkrDeck deck;
	uint8_t playerCount;
	PkrPlayer *pPlayers;
	uint32_t pot;
	uint32_t currentBet;
	uint8_t highestBetter;
} PkrGame;

void pkrPlaceBet(PkrGame *pGame, const uint8_t playerIdx, const uint32_t bet);
uint32_t pkrDecisionStdio(void *pGame, const uint8_t playerIdx);
uint32_t pkrDecisionAuto(void *pGame, const uint8_t playerIdx);
void pkrPlayerTurn(PkrGame *pGame, const uint8_t playerIdx);
void pkrBetRound(PkrGame *pGame, const uint8_t dealSize);
uint8_t pkrDefaultWinner(PkrGame *pGame);
int pkrScanf(const char *pFormat, ...);

#endif  // POKER_PLAYER_H_
