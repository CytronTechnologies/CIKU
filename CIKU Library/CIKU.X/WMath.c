/*
 * File: WMath.h
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on June 17, 2014, 2:40 PM
 */

#include "Arduino.h"

UINT16 makeWord(UINT16 w) { return w; }
UINT16 makeWord2(BYTE h, BYTE l) { return (h << 8) | l; }

long random(long howbig)
{
  if (howbig == 0) {
    return 0;
  }
  return rand() % howbig;
}

long random2(long howsmall, long howbig)
{
  if (howsmall >= howbig) {
    return howsmall;
  }
  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}

void randomSeed(unsigned int seed)
{
  if (seed != 0) {
    srand(seed);
  }
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
