#include "Arduino.h"
#include "G15.h"
#include "PS2.h"
#include "SoftwareSerial.h"

void setup()
{
  G15_begin(19200, 9, 8);
  PS2_begin(9600, 2, 3);
  pinMode(LED, OUTPUT);

  G15_setWheelMode(1);
}

void loop()
{
  if(PS2_getVal(p_up) == PRESS)
  {
    G15_setLED(1, ON, iWRITE_DATA);
    G15_setWheelSpeed(1, 1000, CW);
    digitalWrite(LED, HIGH);
    while(PS2_getVal(p_up) == PRESS);
  }
  else if(PS2_getVal(p_down) == PRESS)
  {
    G15_setLED(1, ON, iWRITE_DATA);
    G15_setWheelSpeed(1, 1000, CCW);
    digitalWrite(LED, HIGH);
    while(PS2_getVal(p_down) == PRESS);
  }
  else
  {
    G15_setLED(1, OFF, iWRITE_DATA);
    G15_setWheelSpeed(1, 0, CW);
    digitalWrite(LED, LOW);
    while(PS2_getVal(p_up) == RELEASE &&
          PS2_getVal(p_down) == RELEASE);
  }
}
