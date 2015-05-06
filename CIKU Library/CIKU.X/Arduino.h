/* 
 * File: Arduino.h
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 23, 2014, 9:56 AM
 */

#ifndef ARDUINO_H
#define	ARDUINO_H

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <GenericTypeDefs.h>

#define _XTAL_FREQ 48000000

#define HIGH 1
#define LOW  0

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define true  1
#define false 0

#define SERIAL  0
#define DISPLAY 1

#define LSBFIRST 0
#define MSBFIRST 1

#define FALLING 0
#define RISING  1

#define DEFAULT  0
#define EXTERNAL 1

#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

#define interrupts() ei()
#define noInterrupts() di()

#define clockCyclesPerMicrosecond()  (_XTAL_FREQ/4/1000000L)
#define clockCyclesToMicroseconds(a) ((a)/clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a)*clockCyclesPerMicrosecond())

void init(void);

void pinMode(UINT8, UINT8);
void digitalWrite(UINT8, UINT8);
int digitalRead(UINT8);
void digitalToggle(UINT8);
void analogReference(UINT8 mode);
int analogRead(UINT8);
void analogWrite(UINT8, int);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(UINT16 us);
unsigned long pulseIn(UINT8 pin, UINT8 state, unsigned long timeout);

UINT8 shiftIn(UINT8 dataPin, UINT8 clockPin, UINT8 bitOrder);
void shiftOut(UINT8 dataPin, UINT8 clockPin, UINT8 bitOrder, UINT8 val);

void attachInterrupt(UINT8, void (*)(void), int mode);
void detachInterrupt(UINT8);

void setup(void);
void loop(void);

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5

#define _BV(P) (UINT8)(1 << P)

#define NOT_ON_TIMER 0
#define TIMER2A 1
#define TIMER2B 2

void tone(UINT8 _pin, unsigned int frequency, unsigned long duration);
void disableTimer(UINT8 _timer);
void noTone(UINT8 _pin);

UINT16 makeWord(UINT16 w);
UINT16 makeWord2(BYTE h, BYTE l);

// WMath prototypes
long random(long);
long random2(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

#include "pins.h"
#include "variables.h"
#include "wiring_private.h"
#include "HardwareSerial.h"

#endif	/* ARDUINO_H */

