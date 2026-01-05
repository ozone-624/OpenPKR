/*
 *    poker.c
 */

#include "pkr/poker.h"

uint8_t pkrPlayRound(uint8_t playerCount, PkrPlayer *pPlayers) {
  // Allocate and set a deck of cards
  PkrDeck deck = {};
  pkrCreateDeck(deck);

  PkrGame game = {
	.deck = {},
	.playerCount = playerCount,
	.pPlayers = pPlayers,
	.pot = 0,
	.currentBet = 0,  // Optional
	.highestBetter = 0  // Optional
  };

  // Pay the ante (if possible) and deal two cards to each player
  for (uint8_t playerIdx = 0; playerIdx < playerCount; ++playerIdx) {
	if (game.pPlayers[playerIdx].balance >= PKR_ANTE) {
    		game.pPlayers[playerIdx].folded = PKR_FALSE;  // They're back in the game
    		game.pPlayers[playerIdx].balance -= PKR_ANTE;
            game.pot = PKR_ANTE * playerCount;
    		for (uint8_t cardIdx = 0; cardIdx < PKR_PLAYER_SIZE; ++cardIdx) {
      			pkrDealCard(game.deck, PKR_LOCATION_PLAYER(playerIdx));
    		}
	} else
        game.pPlayers[playerIdx].folded = PKR_TRUE;  // They can't afford to play
  }

    if (pkrDefaultWinner(&game)) {
        pkrDeclareWinner(&game);
        return 1;
    }

  pkrBetRound(&game, 0);
  if (pkrDefaultWinner(&game)) {
	pkrDeclareWinner(&game);
	return 0;
  }

  pkrBetRound(&game, 3);
  if (pkrDefaultWinner(&game)) {
	pkrDeclareWinner(&game);
	return 0;
  }

  pkrBetRound(&game, 1);
  if (pkrDefaultWinner(&game)) {
	pkrDeclareWinner(&game);
	return 0;
  }

  pkrBetRound(&game, 1);

  pkrDeclareWinner(&game);
  return 0;
}

uint8_t pkrPlayGame(uint8_t playerCount, PkrDecision *pDecisions) {
	PkrPlayer pPlayers[playerCount] = {};
 
	// Initialize values
	for (uint8_t playerIdx = 0; playerIdx < playerCount; ++playerIdx) {
		pPlayers[playerIdx].folded = PKR_FALSE;
  		pPlayers[playerIdx].balance = PKR_INITIAL_CHIPS;
		pPlayers[playerIdx].pDecisionCallback = pDecisions[playerIdx];
	}

	while (pkrPlayRound(playerCount, pPlayers) == 0) {
        printf("Play again? [Y/N] ");

        char choice = '\0';
        pkrScanf("%c", &choice);

        if (choice == 'N' || choice == 'n')
            break;
	}

    return 0;
}

uint8_t pkrDeclareWinner(PkrGame *pGame) {
	uint8_t winnerIdx = 0;	

	if (pkrDefaultWinner(pGame)) {
  		for (uint8_t playerIdx = 0; playerIdx < pGame->playerCount; ++playerIdx) {
			if (!pGame->pPlayers[playerIdx].folded) {
				winnerIdx = playerIdx;
				break;
			}
		}
  	}

	PkrRanking pRankings[pGame->playerCount] = {};
  	for (uint8_t playerIdx = 0; playerIdx < pGame->playerCount; ++playerIdx) {
   		 if (pGame->pPlayers[playerIdx].folded)
			continue;

    		pRankings[playerIdx] = pkrRankHand(pGame->deck, PKR_LOCATION_PLAYER(playerIdx));

   		 if (pRankings[playerIdx].rankType > pRankings[winnerIdx].rankType)
      			winnerIdx = playerIdx;
    		else if (pRankings[playerIdx].rankType == pRankings[winnerIdx].rankType) {
     				if (pRankings[playerIdx].level > pRankings[winnerIdx].level)
       					winnerIdx = playerIdx;
  		}
	}

	printf("Winning player: %u\n", winnerIdx);
	pkrDisplayCards(pGame->deck, PKR_LOCATION_PLAYER(winnerIdx));

    pGame->pPlayers[winnerIdx].balance += pGame->pot;
    for (uint8_t playerIdx = 0; playerIdx < pGame->playerCount; ++playerIdx) {  // Some bets may be placed but not confirmed, collect those, too
        pGame->pPlayers[winnerIdx].balance += pGame->pPlayers[playerIdx].bet;
    }

	return winnerIdx;
}