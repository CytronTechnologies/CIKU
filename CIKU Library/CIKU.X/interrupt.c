/*
 * File: interrupt.c
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 27, 2014
 */

#include "Arduino.h"
#include "wiring_private.h"
#include "HardwareSerial.h"
#include "SoftwareSerial.h"

static unsigned char timer0_fract;
ring_buffer rx_buffer;
ring_buffer tx_buffer;

char INTFlag = 0;

static volatile voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS];

void attachInterrupt(UINT8 interruptNum, void (*userFunc)(void), int mode)
{
  if(interruptNum < EXTERNAL_NUM_INTERRUPTS)
  {
    intFunc[interruptNum] = userFunc;

    switch(interruptNum)
    {
    case 0:
      pinMode(15, INPUT);
      INT0IE = 1; // Always high priority
      INTEDG0 = mode;
      break;

    case 1:
      pinMode(16, INPUT);
      INT1IE = 1;
      INTEDG1 = mode;
      INT1IP = 1; // High priority
      break;

    case 2:
      pinMode(2, INPUT);
      INT2IE = 1;
      INTEDG2 = mode;
      INT2IP = 1; // High priority
      INTFlag = 1;
      break;
    }
  }
}

void detachInterrupt(UINT8 interruptNum)
{
  if(interruptNum < EXTERNAL_NUM_INTERRUPTS)
  {
    switch(interruptNum)
    {
    case 0:
      INT0IE = 0; // Disable interrupt
      break;

    case 1:
      INT1IE = 0; // Disable interrupt
      break;

    case 2:
      INT2IE = 0; // Disable interrupt
      INTFlag = 0;
      break;
    }

    intFunc[interruptNum] = 0;
  }
}

void interrupt high_isr()
{
  if(TMR0IF)
  {
    TMR0IF = 0;

    unsigned long m = timer0_millis;
    unsigned char f = timer0_fract;

    m += MILLIS_INC;
    f += FRACT_INC;
    if(f >= FRACT_MAX)
    {
      f -= FRACT_MAX;
      m += 1;
    }

    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
  }

  if(RCIE && RCIF)
  {
    unsigned char c = RCREG;
    store_char(c, &rx_buffer);
  }

  if(INT0IE && INT0IF)
  {
    if(intFunc[EXTERNAL_INT_0]) intFunc[EXTERNAL_INT_0]();
    INT0IF = 0;
  }

  if(INT1IE && INT1IF)
  {
    if(intFunc[EXTERNAL_INT_1]) intFunc[EXTERNAL_INT_1]();
    INT1IF = 0;
  }

  if(INT2IE && INT2IF)
  {
    if(INTFlag)
    {
      if(intFunc[EXTERNAL_INT_2]) intFunc[EXTERNAL_INT_2]();
      INT2IF = 0;
    }
    else
    {
      SSerial_recv();
      INT2IF = 0;
    }
  }
}

void low_priority interrupt low_isr()
{
  if(TMR1IE && TMR1IF)
  {
    TMR1IF = 0;
    TMR1 = ocr;
    if(timer1_toggle_count != 0)
    {
      // toggle the pin
      *timer1_pin_port ^= timer1_pin_mask;

      if(timer1_toggle_count > 0)
        timer1_toggle_count--;
    }
    else
    {
      disableTimer(1);
      *timer1_pin_port &= ~(timer1_pin_mask);  // keep pin low after stop
    }
  }

  if(TXIE && TXIF)
  {
    if(tx_buffer.head == tx_buffer.tail)
    {
      // Buffer empty, so disable interrupts
      TXIE = 0;
    }
    else
    {
      // There is more data in the output buffer. Send the next byte
      unsigned char c = tx_buffer.buffer[tx_buffer.tail];
      tx_buffer.tail = (tx_buffer.tail + 1) % SERIAL_BUFFER_SIZE;

      TXREG = c;
    }
  }
}
