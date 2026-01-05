#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pkr/texas_holdem.h>

int main(void) {
  srand(time(NULL));
  
  PkrDecision pDecisions[2] = {pkrDecisionStdio, pkrDecisionAuto};

  pkrPlayGame(2, pDecisions);
  
  return 0;

}