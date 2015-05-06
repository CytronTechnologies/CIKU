/*
 * File: wiring_shift.c
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on Jun 12, 2014, 10:09 AM
 */

#include "wiring_private.h"

UINT8 shiftIn(UINT8 dataPin, UINT8 clockPin, UINT8 bitOrder)
{
	UINT8 value = 0;
	UINT8 i;

	for(i = 0; i < 8; ++i)
  {
		digitalWrite(clockPin, HIGH);
		if (bitOrder == LSBFIRST)
			value |= digitalRead(dataPin) << i;
		else
			value |= digitalRead(dataPin) << (7 - i);
		digitalWrite(clockPin, LOW);
	}
	return value;
}

void shiftOut(UINT8 dataPin, UINT8 clockPin, UINT8 bitOrder, UINT8 val)
{
	UINT8 i;

	for(i = 0; i < 8; i++)
  {
		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, !!(val & (1 << i)));
		else
			digitalWrite(dataPin, !!(val & (1 << (7 - i))));

		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}
