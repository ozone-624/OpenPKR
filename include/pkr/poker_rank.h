#ifndef POKER_RANK_H_
#define POKER_RANK_H_

#include "poker_player.h"

typedef struct PkrRanking_T {
  uint8_t rankType;
  int level;
} PkrRanking;

// Documentation needed...
static int pkrMatching(const PkrDeck deck, const uint8_t location);
static int pkrMatchingSecond(const PkrDeck deck, const uint8_t location, uint8_t skipNumber);
static int pkrFlush(const PkrDeck deck, uint8_t location);
static int pkrStraight(const PkrDeck deck, uint8_t location);
static int pkrStraightFlush(const PkrDeck deck, uint8_t location);
static int pkrRankOutsidePair(const PkrDeck deck, uint8_t location, uint8_t pairNum);
static int pkrRankHigh(const PkrDeck deck, uint8_t location);

// Documentation needed...
PkrRanking pkrRankHand(const PkrDeck deck, uint8_t location);

#endif  // POKER_RANK_H_
