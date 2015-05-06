/*
 * File: wiring_digital.c
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 23, 2014, 10:47 AM
 */

#include "pins.h"
#include "wiring_private.h"

void pinMode(UINT8 pin, UINT8 mode)
{
  UINT8 _bit = digitalPinToBitMask[pin];
  UINT8 port = digitalPinToPort[pin];
  volatile UINT8 *reg;

  if(port == NOT_A_PORT) return;

  reg = portModeRegister[port];

  if(mode == INPUT)
  {
    UINT8 oldGIE = GIE;
    di();
    *reg |= _bit;
    GIE = oldGIE;
  }
  else
  {
    UINT8 oldGIE = GIE;
    di();
    *reg &= ~_bit;
    GIE = oldGIE;
  }
}

void turnOffPWM(UINT8 timer)
{
  switch(timer)
  {
  case 1:
    CCP1CONbits.CCP1M = 0;
    break;
    
  case 2:
    CCP2CONbits.CCP2M = 0;
    break;
  }
}

void digitalWrite(UINT8 pin, UINT8 val)
{
  UINT8 timer = digitalPinToTimer[pin];
  UINT8 _bit = digitalPinToBitMask[pin];
  UINT8 port = digitalPinToPort[pin];
  volatile UINT8 *out;

  if(port == NOT_A_PORT) return;

  if(timer != NOT_ON_TIMER) turnOffPWM(timer);

  out = portOutputRegister[port];

  UINT8 oldGIE = GIE;
  di();
  if(val == LOW)
  {
    *out &= ~_bit;
  }
  else
  {
    *out |= _bit;
  }
  GIE = oldGIE;
}

int digitalRead(UINT8 pin)
{
  UINT8 _bit = digitalPinToBitMask[pin];
  UINT8 port = digitalPinToPort[pin];

  if(port == NOT_A_PORT) return LOW;

  if(*portInputRegister[port] & _bit) return HIGH;
  else return LOW;
}

void digitalToggle(UINT8 pin)
{
  *portOutputRegister[digitalPinToPort[pin]] ^= digitalPinToBitMask[pin];
}
