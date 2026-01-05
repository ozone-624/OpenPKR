/*
 *    poker_player.c
 */

#include "pkr/poker_player.h"

void pkrPlaceBet(PkrGame *pGame, const uint8_t playerIdx, const uint32_t bet) {
	uint32_t actualBet = bet;
	
	if (actualBet < pGame->currentBet) {
		pGame->pPlayers[playerIdx].bet = 0;
		pGame->pPlayers[playerIdx].folded = PKR_TRUE;
		return;
	} else if (actualBet > pGame->currentBet) {  // Player is raising
		if (pGame->highestBetter > playerIdx)
			actualBet = pGame->currentBet;
		else {
			// Do not allow the bet to exceed someone's wallet
			// Go from 0 to playerIdx
			for (uint8_t otherPlayerIdx = 0; otherPlayerIdx < pGame->playerCount; ++otherPlayerIdx) {
				if (actualBet > pGame->pPlayers[otherPlayerIdx].balance)
					actualBet = pGame->pPlayers[otherPlayerIdx].balance;
			}

			if (actualBet > pGame->currentBet) {  // Is the player still raising?
				pGame->currentBet = actualBet;
				pGame->highestBetter = playerIdx;
			}
		}
	}

	pGame->pPlayers[playerIdx].bet = actualBet;
}

uint32_t pkrDecisionStdio(void *pGame, const uint8_t playerIdx) {
    PkrGame *pRealGame = (PkrGame*)pGame;  // This variable name sucks

    if (playerIdx >= pRealGame->highestBetter)
        printf("======= NEW BETTING ROUND =======\n");


    printf("River:\n");
    pkrDisplayCards(pRealGame->deck, PKR_LOCATION_RIVER);

    printf("Your hand:\n");
    pkrDisplayCards(pRealGame->deck, PKR_LOCATION_PLAYER(playerIdx));

    printf("Your balance: %u\n\n", pRealGame->pPlayers[playerIdx].balance);

    // Print other balances here
    printf("Other players:\n");

    for (uint8_t otherIdx = 0; otherIdx < pRealGame->playerCount; ++otherIdx) {
        if (otherIdx == playerIdx)
            continue;

        printf("  [Player %u]: (Folded: %s) (Balance: %u) (Bet: %u)\n", otherIdx + 1,
               pRealGame->pPlayers[otherIdx].folded ? "FOLDED" : "NOT FOLDED", pRealGame->pPlayers[otherIdx].balance,
               pRealGame->pPlayers[otherIdx].bet);
    }

    printf("\n");

    printf("Current bet: %u\n", pRealGame->currentBet);
    printf("Current pot: %u\n", pRealGame->pot);

    printf("Options:\n");

    if (pRealGame->currentBet > 0) {
        printf("[1]: Fold\n");
        if (playerIdx >= pRealGame->highestBetter)
            printf("[2]: Raise Bet\n");
        else
            printf("[2]: Check\n");
    } else {
        printf("[1]: Check\n");
        printf("[2]: Place Bet\n");
    }

    uint32_t playerBet = 0;
    char choice = '\0';

    printf("Your choice: ");
    pkrScanf("%c", &choice);

    if (pRealGame->currentBet > 0) {
        if (choice == '1')
            playerBet = 0;  // Fold

        else if (choice == '2') {

            if (playerIdx >= pRealGame->highestBetter) {
                printf("What is the new bet? ");  // Raise bet
                pkrScanf("%u", &playerBet);
            } else
                playerBet = pRealGame->currentBet;  // Check
        }

    } else {
        if (choice == '1')
            playerBet = pRealGame->currentBet;  // Check
        else if (choice == '2') {
            printf("What is the bet? ");  // Place bet
            pkrScanf("%u", &playerBet);
        }
    }

    pkrPlaceBet(pGame, playerIdx, playerBet);
}

uint32_t pkrDecisionAuto(void *pGame, const uint8_t playerIdx) {
	PkrGame *pRealGame = (PkrGame*)pGame;  // This variable name sucks

	uint8_t botIsBetting = rand() % 2;
    if (!botIsBetting)
        printf("Player %u is %sing!\n", playerIdx + 1, pRealGame->currentBet > 0 ? "fold" : "check");

	uint32_t bet = botIsBetting ? pRealGame->currentBet + (rand() % 10) : 0;      

      pkrPlaceBet(pGame, playerIdx, bet);
}

void pkrPlayerTurn(PkrGame *pGame, const uint8_t playerIdx) {
  if (pkrDefaultWinner(pGame))
	return;

  if (!pGame->pPlayers[playerIdx].folded)
	pGame->pPlayers[playerIdx].pDecisionCallback(pGame, playerIdx);
}

void pkrBetRound(PkrGame *pGame, const uint8_t dealSize) {
  if (dealSize) {
    pkrDealCard(pGame->deck, PKR_LOCATION_BURNED);  // Burn a card
    for (uint8_t cardIdx = 0; cardIdx < dealSize; ++cardIdx)
      pkrDealCard(pGame->deck, PKR_LOCATION_RIVER);  // Put some cards in the river
  }
  
  pGame->currentBet = 0;
  pGame->highestBetter = 0;
  for (uint8_t playerIdx = 0; playerIdx < pGame->playerCount; ++playerIdx) {
    pkrPlayerTurn(pGame, playerIdx);
  }
  
   for (uint8_t playerIdx = 0; playerIdx < pGame->highestBetter; ++playerIdx) {
    pkrPlayerTurn(pGame, playerIdx);
  }

  for (uint8_t playerIdx = 0; playerIdx < pGame->playerCount; ++playerIdx) {
	const uint32_t playerBet = pGame->pPlayers[playerIdx].bet; 
	pGame->pPlayers[playerIdx].balance -= playerBet;
	pGame->pot += playerBet; 
  }
}

uint8_t pkrDefaultWinner(PkrGame *pGame) {
	uint8_t foldCount = 0;
	for (uint8_t playerIdx = 0; playerIdx < pGame->playerCount; ++playerIdx) {
		if (pGame->pPlayers[playerIdx].folded)
			++foldCount;
	}

	return (foldCount >= pGame->playerCount - 1) ? PKR_TRUE : PKR_FALSE;
}

int pkrScanf(const char *pFormat, ...) {
    va_list args;
    va_start(args, pFormat);

    int returnCode = vscanf(pFormat, args);
    va_end(args);

    // Clear the stdin buffer
    int throwaway = '\0';
    while ((throwaway = getchar()) != '\n' && throwaway != EOF);

    return returnCode;
}