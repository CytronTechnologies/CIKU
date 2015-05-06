/*
 * File: wiring.c
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 23, 2014, 10:47 AM
 */

#include "Arduino.h"

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;

unsigned long millis(void)
{
  unsigned long m;
	UINT8 oldGIE = GIE;

	di();
	m = timer0_millis;
	GIE = oldGIE;

	return m;
}

unsigned long micros(void)
{
  unsigned long m;
	UINT8 oldGIE = GIE, t;

	di();
	m = timer0_overflow_count;
  t = TMR0L;

  if(TMR0IF && (t < 255)) m++;

	GIE = oldGIE;
  TMR0IF = 0;

	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

void delay(unsigned long ms)
{
  do {
    __delay_ms(1);
  } while(ms-- > 0);
}

void delayMicroseconds(UINT16 us)
{
  do {
  __delay_us(1);
  } while(us-- > 0);
}

void init()
{
  // this needs to be called before setup() or some functions won't
	// work there
  ei();

  ADCON0 = 0x00; // Turn off ADC module
  ADCON1 = 0x0F; // Set all analog pins to digital
  ADCON2 = 0x96;

  PORTA = 0;
  PORTB = 0;
  PORTC = 0;
  PORTD = 0;
  PORTE = 0;

  // At initial state, all IO are input
  TRISA = 0b111111;
  TRISB = 0b11111111;
  TRISC = 0b11111111;
  TRISD = 0b11111111;
  TRISE = 0b111;

  // Turn ON RUN LED
  TRISA4 = 0;
  LATA = 0;

  // TIMER0
  T0CON = 0b11000101; // Enable Timer 0, prescaler 64
  TMR0IE = 1; // Enables the TMR0 overflow interrupt
  TMR0IP = 1; // Timer 0 interrupt high priority
  
  PR2 = 0xFF;
  T2CON = 0x06; // 2.44kHz, Timer 2 for PWM

  PEIE = 1; // Enables all low-priority peripheral interrupts
  IPEN = 1; // Enable priority levels on interrupts
}
