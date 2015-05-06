/* 
 * File: variable.h
 * Author: User
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 27, 2014, 12:30 PM
 */

#ifndef VARIABLE_H
#define	VARIABLE_H

/***** TIMER0 INTERRUPT DEFINE AND VARIABLES *****/
// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

extern volatile unsigned long timer0_overflow_count;
extern volatile unsigned long timer0_millis;

/***** TIMER1 INTERRUPT DEFINE AND VARIABLES *****/
extern volatile long timer1_toggle_count;
extern volatile UINT8 *timer1_pin_port;
extern volatile UINT8 timer1_pin_mask;
extern volatile UINT16 ocr;

#endif	/* VARIABLE_H */

