#ifndef POKER_ENUM_H_
#define POKER_ENUM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

#include <time.h>
#include <string.h>

#define PKR_FALSE		     0U
#define PKR_TRUE			 1U

#define PKR_MATH_13_0        1U 
#define PKR_MATH_13_1        (PKR_MATH_13_0 * 13U)
#define PKR_MATH_13_2        (PKR_MATH_13_1 * 13U)
#define PKR_MATH_13_3        (PKR_MATH_13_2 * 13U)
#define PKR_MATH_13_4        (PKR_MATH_13_3 * 13U)

#define PKR_SUIT_COUNT       4U
#define PKR_NUMBER_COUNT     13U
#define PKR_PLAYER_SIZE      2U 
#define PKR_RIVER_SIZE       5U
#define PKR_HAND_SIZE        (PKR_PLAYER_SIZE + PKR_RIVER_SIZE)

// These are the macros you may be interested in changing around
#define PKR_INITIAL_CHIPS    500
#define PKR_ANTE			  10
 
#define PKR_SUIT_CLUBS_N     0U
#define PKR_SUIT_DIAMONDS_N  1U 
#define PKR_SUIT_HEARTS_N    2U
#define PKR_SUIT_SPADES_N    3U
#define PKR_SUIT_NOSUIT_N    4U

#define PKR_SUIT_CLUBS_C     'C'
#define PKR_SUIT_DIAMONDS_C  'D'
#define PKR_SUIT_HEARTS_C    'H'
#define PKR_SUIT_SPADES_C    'S'
#define PKR_SUIT_NOSUIT_C    'N'

#define PKR_CARD_ACE_N       0U
#define PKR_CARD_TWO_N       1U
#define PKR_CARD_THREE_N     2U
#define PKR_CARD_FOUR_N      3U
#define PKR_CARD_FIVE_N      4U
#define PKR_CARD_SIX_N       5U
#define PKR_CARD_SEVEN_N     6U
#define PKR_CARD_EIGHT_N     7U
#define PKR_CARD_NINE_N      8U
#define PKR_CARD_TEN_N       9U
#define PKR_CARD_JACK_N      10U
#define PKR_CARD_QUEEN_N     11U
#define PKR_CARD_KING_N      12U
#define PKR_CARD_NOCARD_N    13U

#define PKR_CARD_RANKED(N) ((N > 0) ? N : 13U)

#define PKR_CARD_ACE_S       "A"
#define PKR_CARD_TWO_S       "2"
#define PKR_CARD_THREE_S     "3"
#define PKR_CARD_FOUR_S      "4"
#define PKR_CARD_FIVE_S      "5"
#define PKR_CARD_SIX_S       "6"
#define PKR_CARD_SEVEN_S     "7"
#define PKR_CARD_EIGHT_S     "8"
#define PKR_CARD_NINE_S      "9"
#define PKR_CARD_TEN_S       "10"
#define PKR_CARD_JACK_S      "J"
#define PKR_CARD_QUEEN_S     "Q"
#define PKR_CARD_KING_S      "K"
#define PKR_CARD_NOCARD_S    "NA"

#define PKR_RANK_HIGH            0U 
#define PKR_RANK_PAIR            1U
#define PKR_RANK_TWO_PAIR        2U
#define PKR_RANK_THREE_OF_A_KIND 3U
#define PKR_RANK_STRAIGHT        4U
#define PKR_RANK_FLUSH           5U
#define PKR_RANK_FULL_HOUSE      6U
#define PKR_RANK_FOUR_OF_A_KIND  7U
#define PKR_RANK_STRAIGHT_FLUSH  8U
// We don't need a royal flush enum

#define PKR_LOCATION_DECK        0U
#define PKR_LOCATION_RIVER       1U
#define PKR_LOCATION_BURNED      2U
#define PKR_LOCATION_PLAYER(N)   (N + 3U)

// Check if a card is accessible from a given location
#define PKR_CARD_ACCESSIBLE(C, L) ((C == PKR_LOCATION_RIVER || C == L))

#define PKR_PLAYER_STDIO         0U
#define PKR_PLAYER_BOT           1U
#define PKR_PLAYER_FOLDED        2U

#endif  // POKER_ENUM_H_
