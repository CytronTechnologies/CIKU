/*
 * File: HardwareSerial.c
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 27, 2014
 */

#include "HardwareSerial.h"

ring_buffer rx_buffer = { { 0 }, 0, 0};
ring_buffer tx_buffer = { { 0 }, 0, 0};

ring_buffer *_rx_buffer;
ring_buffer *_tx_buffer;

bit transmitting;
unsigned long _timeout;
unsigned long _startMillis;

void store_char(unsigned char c, ring_buffer *buffer)
{
  int i = (unsigned int)(buffer->head + 1) % SERIAL_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if(i != buffer->tail) {
    buffer->buffer[buffer->head] = c;
    buffer->head = i;
  }
}

int Serial_timedRead(void)
{
  int c;
  _startMillis = millis();
  do {
    c = Serial_read();
    if (c >= 0) return c;
  } while(millis() - _startMillis < _timeout);
  return -1;     // -1 indicates timeout
}

void Serial_setTimeout(unsigned long timeout)  // sets the maximum number of milliseconds to wait
{
  _timeout = timeout;
}

void HardwareSerial(ring_buffer *rx_buffer, ring_buffer *tx_buffer)
{
  _rx_buffer = rx_buffer;
  _tx_buffer = tx_buffer;
}

void Serial_begin(unsigned long baudrate)
{
  HardwareSerial(&rx_buffer, &tx_buffer);

  BRG16 = 1; // 16 bit baudrate generator
  BRGH = 1; // High speed
  UINT16 n = (UINT16)(((_XTAL_FREQ / baudrate) / 4) - 0.5);
  SPBRG = n & 0x00FF;
  SPBRGH = (n >> 8) & 0x00FF;

  TRISC6 = 0; // Set tx pin as output
  SYNC = 0; // Asynchronous
  SPEN = 1; // Enable serial
  TXIE = 0; // Disable UART transmission interrupt
  TXIP = 0; // Low priority interrupt
  TX9 = 0; // 8 bit
  TXEN = 1; // Enable UART transmission

  TRISC7 = 1; // Set rx pin as input
  RCIE = 1; // Enable UART reception interrupt
  RCIP = 1; // High priority interrupt
  CREN = 1; // Enable UART reception
}

void Serial_end(void)
{
  // Wait for transmission of outgoing data
  while(_tx_buffer->head != _tx_buffer->tail);

  TXEN = 0; // Disable UART transmission
  CREN = 0; // Disable UART reception
  TXIE = 0; // Disable UART transmission interrupt
  RCIE = 0; // Disable UART reception interrupt

  // Clear any received data
  _rx_buffer->head = _rx_buffer->tail;
}

int Serial_available(void)
{
  return (UINT16)(SERIAL_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % SERIAL_BUFFER_SIZE;
}

int Serial_peek(void)
{
  if(_rx_buffer->head == _rx_buffer->tail)
  {
    return -1;
  }
  else
  {
    return _rx_buffer->buffer[_rx_buffer->tail];
  }
}

int Serial_read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if(_rx_buffer->head == _rx_buffer->tail)
  {
    return -1;
  }
  else
  {
    unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
    _rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % SERIAL_BUFFER_SIZE;
    return c;
  }
}

size_t Serial_readBytes(char *buffer, size_t length)
{
  size_t count = 0;
  while(count < length)
  {
    int c = Serial_timedRead();
    if (c < 0) break;
    *buffer++ = (char)c;
    count++;
  }
  return count;
}

size_t Serial_readBytesUntil(char terminator, char *buffer, size_t length)
{
  if(length < 1) return 0;
  size_t index = 0;
  while(index < length)
  {
    int c = Serial_timedRead();
    if (c < 0 || c == terminator) break;
    *buffer++ = (char)c;
    index++;
  }
  return index; // return number of characters, not including null terminator
}

void Serial_flush(void)
{
  // TSR is kept full while the buffer is not empty, so TXC triggers when EMPTY && SENT
  while(transmitting && !TRMT);
  transmitting = false;
}

size_t Serial_write(UINT8 c)
{
  int i = (_tx_buffer->head + 1) % SERIAL_BUFFER_SIZE;

  // If the output buffer is full, there's nothing for it other than to
  // wait for the interrupt handler to empty it a bit
  // ???: return 0 here instead?
  while (i == _tx_buffer->tail);

  _tx_buffer->buffer[_tx_buffer->head] = c;
  _tx_buffer->head = i;

  TXIE = 1; // Enable UART transmission interrupt
  transmitting = true;

  return 1;
}

void Serial_printString(const char *s)
{
    while(*s) Serial_write(*s++);
}

void Serial_printNumber(long n, UINT8 base)
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

  while(*s) Serial_write(*s++);

//  UINT8 i, j, di[10];
//  for(i = 0; i < 10; i++) di[i] = 0;
//  i = 0;
//
//  do {
//    di[i++] = number % base;
//    number = number / base;
//  } while(number);
//
//  for(j = digit; j > 0; j--)
//  {
//    if(di[j - 1] < 10) Serial_write(di[j - 1] + '0');
//    else Serial_write(di[j - 1] - 10 + 'A');
//  }
}

void Serial_printFloat(double number, UINT8 point)
{
  if(number > 4294967040.0) Serial_printString("ovf");  // constant determined empirically
  else if(number <-4294967040.0) Serial_printString ("ovf");  // constant determined empirically
  else
  {
    // Handle negative numbers
    if(number < 0.0)
    {
      Serial_write('-');
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
    Serial_printNumber(int_part, DEC);

    // Print the decimal point, but only if there are digits beyond
    if(point > 0) {
      Serial_write('.');
    }

    // Extract digits from the remainder one at a time
    while(point-- > 0)
    {
      remainder *= 10.0;
      int toPrint = (int)remainder;
      Serial_printNumber(toPrint, DEC);
      remainder -= toPrint;
    }
  }
}
