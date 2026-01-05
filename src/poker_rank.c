/*
 *    poker_rank.c
 */

#include "pkr/poker_rank.h"

// Documentation needed...
// Checks for pairs, three of a kind, and four of a kind
static int pkrMatching(const PkrDeck deck, const uint8_t location) {
  uint8_t highestNumber = 0;
  uint8_t highestMatching = 0;
  
  for (int cardNum = PKR_NUMBER_COUNT - 1; cardNum >= 0; --cardNum) {
    uint8_t matching = 0;
    
    for (int cardSuit = 0; cardSuit < PKR_SUIT_COUNT; ++cardSuit) {
      if (PKR_CARD_ACCESSIBLE(deck[cardSuit][cardNum], location))
        ++matching;
    }
    
    if (matching > highestMatching) {
      highestMatching = matching;
      highestNumber = PKR_CARD_RANKED(cardNum);
    }
  }
  
  return (highestMatching << 8) | highestNumber;
}

// Documentation needed...
// Check for the second-highest match for two-pair and full-house
static int pkrMatchingSecond(const PkrDeck deck, const uint8_t location, uint8_t skipNumber) {
  uint8_t highestNumber = 0;
  uint8_t highestMatching = 0;
  
  for (int cardNum = PKR_NUMBER_COUNT - 1; cardNum >= 0; --cardNum) {
    if (cardNum == skipNumber)
        continue;

    uint8_t matching = 0;
    
    for (int cardSuit = 0; cardSuit < PKR_SUIT_COUNT; ++cardSuit) {
      if (PKR_CARD_ACCESSIBLE(deck[cardSuit][cardNum], location))
        ++matching;
    }
    
    if (matching > highestMatching) {
      highestMatching = matching;
      highestNumber = PKR_CARD_RANKED(cardNum);
    }
  }
  
  return (highestMatching << 8) | highestNumber;
}

static int pkrFlush(const PkrDeck deck, uint8_t location) {
  for (int cardSuit = 0; cardSuit < PKR_SUIT_COUNT; ++cardSuit) {
    uint32_t flushLevel = 0;
    uint8_t flushRating = 0;
    
    for (int cardNum = PKR_NUMBER_COUNT - 1; cardNum >= 0; --cardNum) {
        if (PKR_CARD_ACCESSIBLE(deck[cardSuit][cardNum], location)) {
          flushLevel = PKR_NUMBER_COUNT * flushLevel + PKR_CARD_RANKED(cardNum);
        
          if (++flushRating == 5)
            return flushLevel;
        }
    }
  }
  
  return 0;
}

// Check for a straight
static int pkrStraight(const PkrDeck deck, uint8_t location) {
    for (int cardNum = PKR_CARD_TEN_N; cardNum >= 0; --cardNum) {
        for (int offset = 0; offset < 5U; ++offset) {
          uint8_t offsetNum = cardNum + offset;
          if (offsetNum == PKR_NUMBER_COUNT)
            offsetNum = PKR_CARD_ACE_N;
          
          uint8_t foundCard = 0;
          for (int cardSuit = 0; cardSuit < PKR_SUIT_COUNT; ++cardSuit) {
            if (PKR_CARD_ACCESSIBLE(deck[cardSuit][offsetNum], location)) {
              foundCard = 1;
              break;
            }
          }
          
          if (!foundCard)
            break;
            
          if (offset == 4U)
            return PKR_CARD_RANKED(cardNum);
        }
    }
    
    return -1;
}

static int pkrStraightFlush(const PkrDeck deck, uint8_t location) {
  for (int cardSuit = 0; cardSuit < PKR_SUIT_COUNT; ++cardSuit) {
    for (int cardNum = PKR_CARD_TEN_N; cardNum >= 0; --cardNum) {
        for (int offset = 0; offset < 5U; ++offset) {
          uint8_t offsetNum = cardNum + offset;
          if (offsetNum == PKR_NUMBER_COUNT)
            offsetNum = PKR_CARD_ACE_N;
          
          int foundCard = PKR_CARD_ACCESSIBLE(deck[cardSuit][offsetNum], location);
          
          if (!foundCard)
            break;
            
          if (offset == 4U)
            return PKR_CARD_RANKED(cardNum);
        }
    }
  }
  
  return -1;
}

static int pkrRankOutsidePair(const PkrDeck deck, uint8_t location, uint8_t pairNum) {
  int pairLevel = pairNum;
  uint8_t breakCount = 0;  

  for (int cardNum = PKR_NUMBER_COUNT - 1; cardNum >= 0; --cardNum) {
    if (cardNum == pairNum)
        continue;

    for (int cardSuit = 0; cardSuit < PKR_SUIT_COUNT; ++cardSuit) {
      if (PKR_CARD_ACCESSIBLE(deck[cardSuit][cardNum], location)) {
        pairLevel = 13 * pairLevel + PKR_CARD_RANKED(cardNum);
        ++breakCount;
        break;
      }
    }
    
    if (breakCount == 3)
      break;
  }
  
  return pairLevel;
}

static int pkrRankHigh(const PkrDeck deck, uint8_t location) {
  int highLevel = 0;
  uint8_t breakCount = 0;  

  for (int cardNum = PKR_NUMBER_COUNT - 1; cardNum >= 0; --cardNum) {
    for (int cardSuit = 0; cardSuit < PKR_SUIT_COUNT; ++cardSuit) {
      if (PKR_CARD_ACCESSIBLE(deck[cardSuit][cardNum], location)) {
        highLevel = 13 * highLevel + PKR_CARD_RANKED(cardNum);
        ++breakCount;
        break;
      }
    }
    
    if (breakCount == 5)
      break;
  }
  
  return highLevel;
}

// Documentation needed...
PkrRanking pkrRankHand(const PkrDeck deck, uint8_t location) {
  const int straightFlushLevel = pkrStraightFlush(deck, location);
  if (straightFlushLevel != -1) {
    PkrRanking straightFlushRanking = {
      .rankType = PKR_RANK_STRAIGHT_FLUSH,
      .level = straightFlushLevel
    };
    
    return straightFlushRanking;
  }
  
  const uint16_t matchingLevel = pkrMatching(deck, location);
  if ((matchingLevel >> 8) == 4) {
    PkrRanking fourOfAKindRanking = {
      .rankType = PKR_RANK_FOUR_OF_A_KIND,
      .level = matchingLevel & 0xFF
    };
    
    return fourOfAKindRanking;
  }
  
  const uint16_t secondMatchingLevel = pkrMatchingSecond(deck, location, matchingLevel & 0xFF);
  if ((matchingLevel >> 8) == 3 && (secondMatchingLevel >> 8) == 2) {
    PkrRanking fullHouseRanking = {
      .rankType = PKR_RANK_FULL_HOUSE,
      .level = 13 * (matchingLevel & 0xFF) + (secondMatchingLevel & 0xFF)
    };
    
    return fullHouseRanking;
  }
  
  const int flushLevel = pkrFlush(deck, location);
  if (flushLevel != 0) {
    PkrRanking flushRanking = {
      .rankType = PKR_RANK_FLUSH,
      .level = flushLevel
    };
    
    return flushRanking;
  }
  
  const int straightLevel = pkrStraight(deck, location);
  if (straightLevel != -1) {
    PkrRanking straightRanking = {
      .rankType = PKR_RANK_STRAIGHT,
      .level = straightLevel
    };
    
    return straightRanking;
  }
  
  if ((matchingLevel >> 8) == 3) {
    PkrRanking threeOfAKindRanking = {
      .rankType = PKR_RANK_THREE_OF_A_KIND,
      .level = (matchingLevel & 0xFF)
    };
    
    return threeOfAKindRanking;
  }
  
  if ((secondMatchingLevel >> 8) == 2) {
    PkrRanking twoPairRanking = {
      .rankType = PKR_RANK_TWO_PAIR,
      .level = 13 * (matchingLevel & 0xFF) + (secondMatchingLevel & 0xFF)
    };
    
    return twoPairRanking;
  }
  
  if ((matchingLevel >> 8) == 2) {
    PkrRanking pairRanking = {
      .rankType = PKR_RANK_PAIR,
      .level = pkrRankOutsidePair(deck, location, matchingLevel & 0xFF)
    };
    
    return pairRanking;
  }
  
  PkrRanking highRanking = {
    .rankType = PKR_RANK_HIGH,
    .level = pkrRankHigh(deck, location)
  };
  
  return highRanking;
}

