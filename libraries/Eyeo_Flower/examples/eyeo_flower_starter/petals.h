#ifndef PETALS_H_
#define PETALS_H_

#include "config.h"

// Defines for Petals
#define PETAL12 2     
#define PETAL2  3
#define PETAL3  4
#define PETAL4  5
#define PETAL6  6
#define PETAL8  7
#define PETAL9  8
#define PETAL10 9

const byte allPetals[] =    {PETAL12,PETAL2,PETAL3,PETAL4,PETAL6,PETAL8,PETAL9,PETAL10};
const byte threePetals[] =  {PETAL12,PETAL4,PETAL8};
const byte fourPetals[] =   {PETAL12,PETAL3,PETAL6,PETAL9};
const byte sixPetals[] =    {PETAL12,PETAL2,PETAL4,PETAL6,PETAL8,PETAL10};
byte petalAmount[] = {0,0,0,0,0,0,0,0};

void updatePetals(){
  // stagger pwm counter starts to reduce combined power usage
  static byte pinCount[] = {0,32,64,96,128,160,192,224};
  for(int i=0; i<8; i++){
    (pinCount[i]++ >= petalAmount[i]) ? digitalWrite(allPetals[i], LOW) : digitalWrite(allPetals[i], HIGH);
  }
}

void setPetals(byte p12, byte p2, byte p3, byte p4, byte p6, byte p8, byte p9, byte p10){
  petalAmount[0]=p12; //PETAL12
  petalAmount[1]=p2; //PETAL2
  petalAmount[2]=p3; //PETAL3
  petalAmount[3]=p4; //PETAL4
  petalAmount[4]=p6; //PETAL6
  petalAmount[5]=p8; //PETAL8
  petalAmount[6]=p9; //PETAL9
  petalAmount[7]=p10; //PETAL10
}

void setThreePetals(byte p12, byte p4, byte p8) {
  setPetals(p12, 0, 0, p4, 0, p8, 0, 0);
}

void setFourPetals(byte p12, byte p3, byte p6, byte p9) {
  setPetals(p12, 0, p3, 0, p6, 0, p9, 0);
}

void setSixPetals(byte p12, byte p2, byte p4, byte p6, byte p8, byte p10) {
  setPetals(p12, p2, 0, p4, p6, p8, 0, p10);
}

void setAllPetals(byte amt) {
#if PETAL_COUNT == 3
  setThreePetals(amt,amt,amt);
#elif PETAL_COUNT == 4
  setFourPetals(amt,amt,amt,amt);
#else
  setSixPetals(amt,amt,amt,amt,amt,amt);
#endif
}

#endif /* PETALS_H_ */
