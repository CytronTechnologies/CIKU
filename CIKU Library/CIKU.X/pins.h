/* 
 * File: pins.h
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 23, 2014, 10:47 AM
 */

#ifndef PINS_H
#define	PINS_H

#include "Arduino.h"

#define LED  13
#define AREF 14
#define SDA  15
#define SCL  16
#define A0   17
#define A1   18
#define A2   19
#define A3   20
#define A4   21
#define A5   22
#define SW   23

volatile UINT8 * const portModeRegister[] = {
  NOT_A_PORT,
  &TRISA,
  &TRISB,
  &TRISC,
  &TRISD,
  &TRISE
};

volatile UINT8 * const portInputRegister[] = {
  NOT_A_PORT,
  &PORTA,
  &PORTB,
  &PORTC,
  &PORTD,
  &PORTE
};

volatile UINT8 * const portOutputRegister[] = {
  NOT_A_PORT,
  &LATA,
  &LATB,
  &LATC,
  &LATD,
  &LATE
};

const UINT8 digitalPinToPort[] = {
  PC, // 0 - RC7
  PC, // 1 - RC6
  PB, // 2 - RB2
  PB, // 3 - RB3
  PD, // 4 - RD0
  PC, // 5 - RC2
  PC, // 6 - RC1
  PD, // 7 - RD1

  PD, // 8 - RD2
  PD, // 9 - RD3
  PD, // 10 - RD4
  PD, // 11 - RD5
  PD, // 12 - RD6
  PD, // 13 - RD7

  PA, // 14 - RA3
  PB, // 15 - RB0
  PB, // 16 - RB1

  PA, // 17 - RA0
  PA, // 18 - RA1
  PA, // 19 - RA2
  PA, // 20 - RA5
  PE, // 21 - RE0
  PE, // 22 - RE1

  PC, // 23 - RC0
  PA, // 24 - RA4
  PB, // 25 - RB6
  PB  // 26 - RB7
};

const UINT8 digitalPinToBitMask[] = {
  _BV(7), // 0 - RC7
  _BV(6), // 1 - RC6
  _BV(2), // 2 - RB2
  _BV(3), // 3 - RB3
  _BV(0), // 4 - RD0
  _BV(2), // 5 - RC2
  _BV(1), // 6 - RC1
  _BV(1), // 7 - RD1

  _BV(2), // 8 - RD2
  _BV(3), // 9 - RD3
  _BV(4), // 10 - RD4
  _BV(5), // 11 - RD5
  _BV(6), // 12 - RD6
  _BV(7), // 13 - RD7

  _BV(3), // 14 - RA3
  _BV(0), // 15 - RB0
  _BV(1), // 16 - RB1

  _BV(0), // 17 - RA0
  _BV(1), // 18 - RA1
  _BV(2), // 19 - RA2
  _BV(5), // 20 - RA5
  _BV(0), // 21 - RE0
  _BV(1), // 22 - RE1

  _BV(0), // 23 - RC0
  _BV(4), // 24 - RA4
  _BV(6), // 25 - RB6
  _BV(7)  // 26 - RB7
};

const UINT8 analogPinToChannel[] = {
  0, // A0 - RA0 - AN0
  1, // A1 - RA1 - AN1
  2, // A2 - RA2 - AN2
  4, // A3 - RA5 - AN4
  5, // A4 - RE0 - AN5
  6  // A5 - RE1 - AN6
};

const UINT8 digitalPinToTimer[] = {
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  TIMER2A,
  TIMER2B,
  NOT_ON_TIMER,

  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,

  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,

  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,

  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER
};

#endif	/* PINS_H */

