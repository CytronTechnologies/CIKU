/*
 * File: SoftwareSerial.h
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on June 16, 2014, 2:20 PM
 */

#include "SoftwareSerial.h"

#define _DEBUG_PIN  25 // After finish debug, change to pin 25/26

typedef struct _DELAY_TABLE
{
  long baud;
  unsigned short rx_delay_centering;
  unsigned short rx_delay_intrabit;
  unsigned short rx_delay_stopbit;
  unsigned short tx_delay;
} DELAY_TABLE;

// Need to adjust all the values
static const DELAY_TABLE table[] =
{
  //baud    |rxcenter|rxintra |rxstop  |tx
//  { 115200,  1,       5,       5,       4,   }, // Very bad
//  { 57600,   7,       14,      14,      15,  }, // Bad
//  { 38400,   13,      27,      27,      25,  }, // Not good
  { 19200,   31,      63,      63,      52,  }, // Done
  { 9600,    66,      133,     133,     110, }, // Done
  { 4800,    135,     271,     271,     223, }, // Done
  { 2400,    275,     549,     549,     450, }, // not rx
  { 1200,    552,     1104,    1104,    905, }, // not rx
};

const int XMIT_START_ADJUSTMENT = 0;

static inline void tunedDelay(UINT16 delay)
{
  do {
    delay--;
  } while(delay);
}

void SSerial_recv(void)
{
  UINT8 d = 0;

  if(_inverse_logic ? SSerial_rx_pin_read() : !SSerial_rx_pin_read())
  {
    tunedDelay(_rx_delay_centering);
    digitalToggle(_DEBUG_PIN); // Debug

    for(UINT8 i = 1; i; i <<= 1)
    {
      tunedDelay(_rx_delay_intrabit);
      digitalToggle(_DEBUG_PIN); // Debug
      UINT8 noti = ~i;
      if(SSerial_rx_pin_read())
        d |= i;
      else // else clause added to ensure function timing is ~balanced
        d &= noti;
    }

    // skip the stop bit
    tunedDelay(_rx_delay_stopbit);
    digitalToggle(_DEBUG_PIN); // Debug

    if(_inverse_logic)
      d = ~d;

    // if buffer full, set the overflow flag and return
    if((_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF != _receive_buffer_head)
    {
      // save new data in buffer: tail points to where byte goes
      _receive_buffer[_receive_buffer_tail] = d; // save new byte
      _receive_buffer_tail = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
    }
    else
    {
//      DebugPulse(_DEBUG_PIN1, 1);
      _buffer_overflow = true;
    }
  }
}

void SSerial_tx_pin_write(UINT8 pin_state)
{
  if(pin_state == LOW)
    *_transmitPortRegister &= ~_transmitBitMask;
  else
    *_transmitPortRegister |= _transmitBitMask;
}

UINT8 SSerial_rx_pin_read()
{
  return *_receivePortRegister & _receiveBitMask;
}

void SoftwareSerial(UINT8 receivePin, UINT8 transmitPin)
{
  _rx_delay_centering = _rx_delay_intrabit = _rx_delay_stopbit = _tx_delay = 0;
  SSerial_setTx(transmitPin);
  SSerial_setRx(receivePin);

  INTEDG2 = FALLING;
  INT2IE = 1; // Enable interrupt
  INT2IP = 1; // High priority
}

void SSerial_setTx(UINT8 tx)
{
  pinMode(tx, OUTPUT);
  digitalWrite(tx, HIGH);
  _transmitBitMask = digitalPinToBitMask[tx];
  UINT8 port = digitalPinToPort[tx];
  _transmitPortRegister = portOutputRegister[port];
}

void SSerial_setRx(UINT8 rx)
{
  pinMode(rx, INPUT);
  if (!_inverse_logic)
    RBPU = 0;  // pullup for normal logic!
  _receivePin = rx;
  _receiveBitMask = digitalPinToBitMask[rx];
  UINT8 port = digitalPinToPort[rx];
  _receivePortRegister = portInputRegister[port];
}

void SSerial_begin(long speed)
{
  SoftwareSerial(2, 3);

  for(unsigned i = 0; i < sizeof(table)/sizeof(table[0]); ++i)
  {
    long baud = table[i].baud;
    if(baud == speed)
    {
      _rx_delay_centering = table[i].rx_delay_centering;
      _rx_delay_intrabit = table[i].rx_delay_intrabit;
      _rx_delay_stopbit = table[i].rx_delay_stopbit;
      _tx_delay = table[i].tx_delay;
      break;
    }
  }

  tunedDelay(_tx_delay);

  _buffer_overflow = false;
  _receive_buffer_head = _receive_buffer_tail = 0;
}

void SSerial_end(void)
{
  INT2IE = 0; // Disable interrupt
}

int SSerial_read(void)
{
  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  UINT8 d = _receive_buffer[_receive_buffer_head]; // grab next byte
  _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  return d;
}

int SSerial_available(void)
{
  return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) % _SS_MAX_RX_BUFF;
}

size_t SSerial_write(UINT8 b)
{
  if(_tx_delay == 0) {
//    setWriteError();
    return 0;
  }

  UINT8 oldGIE = GIE;
  di();  // Turn off interrupts for a clean txmit

  // Write the start bit
  SSerial_tx_pin_write(_inverse_logic ? HIGH : LOW);
  tunedDelay(_tx_delay + XMIT_START_ADJUSTMENT);

  // Write each of the 8 bits
  if(_inverse_logic)
  {
    for (BYTE mask = 0x01; mask; mask <<= 1)
    {
      if(b & mask) // choose bit
        SSerial_tx_pin_write(LOW); // send 1
      else
        SSerial_tx_pin_write(HIGH); // send 0

      tunedDelay(_tx_delay);
    }

    SSerial_tx_pin_write(LOW); // restore pin to natural state
  }
  else
  {
    for (BYTE mask = 0x01; mask; mask <<= 1)
    {
      if(b & mask) // choose bit
        SSerial_tx_pin_write(HIGH); // send 1
      else
        SSerial_tx_pin_write(LOW); // send 0

      tunedDelay(_tx_delay);
    }

    SSerial_tx_pin_write(HIGH); // restore pin to natural state
  }

  GIE = oldGIE; // turn interrupts back on
  tunedDelay(_tx_delay);

  return 1;
}

void SSerial_flush(void)
{
  UINT8 oldGIE = GIE;
  di();
  _receive_buffer_head = _receive_buffer_tail = 0;
  GIE = oldGIE;
}

int SSerial_peek(void)
{
  // Empty buffer?
  if(_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  return _receive_buffer[_receive_buffer_head];
}

void SSerial_printString(const char *s)
{
  while(*s) SSerial_write(*s++);
}

void SSerial_printNumber(long n, UINT8 base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *s = &buf[sizeof(buf) - 1];

  *s = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    unsigned long m = n;
    n /= base;
    char c = m - base * n;
    *--s = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  while(*s) SSerial_write(*s++);
}

void SSerial_printFloat(double number, UINT8 point)
{
  if(number > 4294967040.0) SSerial_printString("ovf");  // constant determined empirically
  else if(number <-4294967040.0) SSerial_printString ("ovf");  // constant determined empirically
  else
  {
    // Handle negative numbers
    if(number < 0.0)
    {
      SSerial_write('-');
      number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for(UINT8 i = 0; i < point; ++i)
      rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    SSerial_printNumber(int_part, DEC);

    // Print the decimal point, but only if there are digits beyond
    if(point > 0) {
      SSerial_write('.');
    }

    // Extract digits from the remainder one at a time
    while(point-- > 0)
    {
      remainder *= 10.0;
      int toPrint = (int)remainder;
      SSerial_printNumber(toPrint, DEC);
      remainder -= toPrint;
    }
  }
}
