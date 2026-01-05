/*
 *    poker_card.c
 */

#include "pkr/playing_card.h"

// Documentation needed...
void pkrCreateDeck(PkrDeck deck) {
  for (int suit = 0; suit < PKR_SUIT_COUNT; ++suit) {
    for (int num = 0; num < PKR_NUMBER_COUNT; ++num) {
      deck[suit][num] = 0;
    }
  }
}

void pkrDealCard(PkrDeck deck, uint8_t location) {
  uint8_t randomSuit = rand() % PKR_SUIT_COUNT;
  uint8_t randomNum = rand() % PKR_NUMBER_COUNT;
  
  // Keep searching through the deck until we find a card actually in the deck
  while (deck[randomSuit][randomNum] != PKR_LOCATION_DECK) {
    ++randomNum;
    
    if (randomNum == PKR_NUMBER_COUNT) {
      randomNum = 0;
      ++randomSuit;
    }
    
    if (randomSuit == PKR_SUIT_COUNT)
      randomSuit = 0;
  }
  
  deck[randomSuit][randomNum] = location;  // We have a card, deal it
}

void pkrDisplayCards(const PkrDeck deck, uint8_t location) {
  for (uint8_t cardSuit = 0; cardSuit < PKR_SUIT_COUNT; ++cardSuit) {
    for (uint8_t cardNum = 0; cardNum < PKR_NUMBER_COUNT; ++cardNum) {
      if (deck[cardSuit][cardNum] == location)
        printf("%s%c ", ppCardNumbers[cardNum], pSuits[cardSuit]);
    }
  }
  printf("\n");
}
