/*
 * File: wiring_analog.c
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 23, 2014, 10:47 AM
 */

#include "pins.h"

UINT8 analog_reference = DEFAULT;

void analogReference(UINT8 mode)
{
  analog_reference = mode;
}

int analogRead(UINT8 pin)
{
  pin -= 17;

  pin = analogPinToChannel[pin];

  ADCON1bits.PCFG = ~(pin+1);
  ADCON0bits.CHS = pin;
  ADCON1bits.VCFG0 = analog_reference;

  ADON = 1; // Enable ADC module

  GO = 1; // Start conversion
  while(GO) continue; // Wait until the conversion is complete
  delay(1);

  int val = 0;
  val = ADRESH;
  val = val << 8;
  val |= ADRESL;

  ADON = 0; // Disable ADC module
  ADCON1bits.PCFG = 0b1111; // Set all analog pins to digital

  return val;
}

void analogWrite(UINT8 pin, int val) // 10 bits
{
  pinMode(pin, OUTPUT);
  
  if(val == 0) digitalWrite(pin, LOW);
	else if(val >= 1023) digitalWrite(pin, HIGH);
  else
  {
    switch(digitalPinToTimer[pin])
    {
    case TIMER2A:
      CCP1CONbits.CCP1M = 0b1100; // Enable PWM
      CCPR1L = (val >> 2) & 0xFF;
      CCP1CONbits.DC1B = val & 3;
      break;

    case TIMER2B:
      CCP2CONbits.CCP2M = 0b1100;
      CCPR2L = (val >> 2) & 0xFF;
      CCP2CONbits.DC2B = val & 3;
      break;

    case NOT_ON_TIMER:
    default:
      if(val < 500) digitalWrite(pin, LOW);
      else digitalWrite(pin, HIGH);
    }
  }
}
