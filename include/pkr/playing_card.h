#ifndef POKER_CARD_H_
#define POKER_CARD_H_

#include "pkr_enum.h"

// Documentation needed...
const static char pSuits[PKR_SUIT_COUNT] = {
  PKR_SUIT_CLUBS_C,
  PKR_SUIT_DIAMONDS_C,
  PKR_SUIT_HEARTS_C,
  PKR_SUIT_SPADES_C
};

// Documentation needed...
const static char ppCardNumbers[PKR_NUMBER_COUNT][3] = {
  PKR_CARD_ACE_S,
  PKR_CARD_TWO_S,
  PKR_CARD_THREE_S,
  PKR_CARD_FOUR_S,
  PKR_CARD_FIVE_S,
  PKR_CARD_SIX_S,
  PKR_CARD_SEVEN_S,
  PKR_CARD_EIGHT_S,
  PKR_CARD_NINE_S,
  PKR_CARD_TEN_S,
  PKR_CARD_JACK_S,
  PKR_CARD_QUEEN_S,
  PKR_CARD_KING_S
};

// Documentation needed...
typedef uint8_t PkrCard;
typedef PkrCard PkrDeck[PKR_SUIT_COUNT][PKR_NUMBER_COUNT];

void pkrCreateDeck(PkrDeck deck);
void pkrDealCard(PkrDeck deck, uint8_t location);
void pkrDisplayCards(const PkrDeck deck, uint8_t location);


#endif  // POKER_CARD_H_
