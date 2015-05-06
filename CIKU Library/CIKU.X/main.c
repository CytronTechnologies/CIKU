/* 
 * File: main.c
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 23, 2014, 9:35 AM
 */

#include "Arduino.h"

void main(void)
{
  init();

  setup();

  while(1)
  {
    loop();
  }
}

