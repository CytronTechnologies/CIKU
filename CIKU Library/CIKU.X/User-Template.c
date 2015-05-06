/*
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 */

#include "Arduino.h"

void setup()
{
  pinMode(LED, OUTPUT);
}

void loop()
{
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}
