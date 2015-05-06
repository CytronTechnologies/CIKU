/* 
 * File: HardwareSerial.h
 * Author: Idris, Cytron Technologies
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on May 27, 2014, 2:44 PM
 */

#ifndef HARDWARESERIAL_H
#define	HARDWARESERIAL_H

#include "Arduino.h"

#define Serial_println() Serial_printString("\r\n")

#define SERIAL_BUFFER_SIZE 64

typedef struct
{
  unsigned char buffer[SERIAL_BUFFER_SIZE];
  volatile unsigned int head;
  volatile unsigned int tail;
} ring_buffer;

extern ring_buffer rx_buffer;
extern ring_buffer tx_buffer;

void store_char(unsigned char, ring_buffer *);
int Serial_timedRead(void);
void Serial_setTimeout(unsigned long timeout);
void HardwareSerial(ring_buffer *, ring_buffer *);
void Serial_begin(unsigned long);
void Serial_end(void);
int Serial_available(void);
int Serial_peek(void);
int Serial_read(void);
size_t Serial_readBytes(char *buffer, size_t length);
size_t Serial_readBytesUntil(char terminator, char *buffer, size_t length);
void Serial_flush(void);
size_t Serial_write(UINT8);
void Serial_printString(const char *);
void Serial_printNumber(long, UINT8);
void Serial_printFloat(double, UINT8);

#endif	/* HARDWARESERIAL_H */

