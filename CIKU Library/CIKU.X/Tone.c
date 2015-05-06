/*
 * File: Tone.c
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 27, 2014, 9:37 AM
 */

#include "Arduino.h"

volatile long timer1_toggle_count;
volatile UINT8 *timer1_pin_port;
volatile UINT8 timer1_pin_mask;
volatile UINT16 ocr = 0;

#define AVAILABLE_TONE_PINS 1

const UINT8 tonePinToTimer[] = {1};
static UINT8 tone_pins[AVAILABLE_TONE_PINS] = {255};

static UINT8 toneBegin(UINT8 _pin)
{
  INT8 _timer = -1;

  tone_pins[0] = _pin;
  _timer = tonePinToTimer[0];

  if(_timer != -1)
  {
    switch(_timer)
    {
    // Always use Timer1 for Tone
    // Only 1 pin per time
    case 1:
      T1CON = 0b10110001; // Prescaler 1:8
      timer1_pin_port = portOutputRegister[digitalPinToPort[_pin]];
      timer1_pin_mask = digitalPinToBitMask[_pin];
      break;
    }
  }

  return _timer;
}

void tone(UINT8 _pin, unsigned int frequency, unsigned long duration)
{
  long toggle_count = 0;
  
  INT8 _timer;

  _timer = toneBegin(_pin);

  if(_timer >= 0)
  {
    // Set the pinMode as OUTPUT
    pinMode(_pin, OUTPUT);

    if(_timer == 1)
    {
      ocr = _XTAL_FREQ / 4 / 8 / frequency;
      ocr = 65535 - ocr;
    }

    // Calculate the toggle count
    if (duration > 0)
    {
      toggle_count = 2 * frequency * duration / 1000;
    }
    else
    {
      toggle_count = -1;
    }

    switch (_timer)
    {
    case 1:
      TMR1 = ocr;
      timer1_toggle_count = toggle_count;
      TMR1IP = 0; // Low priority
      TMR1IE = 1; // Enable Timer1 interrupt
      break;
    }
  }
}

void disableTimer(UINT8 _timer)
{
  switch (_timer)
  {
  case 1:
    TMR1IE = 0; // Disable Timer1 interrupt
    TMR1ON = 0; // Stops Timer1
    break;
  }
}

void noTone(UINT8 _pin)
{
  INT8 _timer = -1;

  for(int i = 0; i < AVAILABLE_TONE_PINS; i++)
  {
    if(tone_pins[i] == _pin)
    {
      _timer = tonePinToTimer[0];
      tone_pins[i] = 255;
    }
  }

  disableTimer(_timer);

  digitalWrite(_pin, LOW);
}
