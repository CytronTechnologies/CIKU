/* 
 * File: SoftwareSerial.h
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on June 16, 2014, 2:20 PM
 *
 * Baudrate support: 1200, 2400, 4800, 9600, 19200
 */

#ifndef SOFTWARESERIAL_H
#define	SOFTWARESERIAL_H

#include "Arduino.h"

#define _SS_MAX_RX_BUFF 64 // RX buffer size

UINT8 _receivePin;
UINT8 _receiveBitMask;
volatile UINT8 *_receivePortRegister;
UINT8 _transmitBitMask;
volatile UINT8 *_transmitPortRegister;

UINT16 _rx_delay_centering;
UINT16 _rx_delay_intrabit;
UINT16 _rx_delay_stopbit;
UINT16 _tx_delay;

UINT16 _inverse_logic = 0;
UINT16 _buffer_overflow;

char _receive_buffer[_SS_MAX_RX_BUFF];
volatile UINT16 _receive_buffer_tail;
volatile UINT16 _receive_buffer_head;

static inline void tunedDelay(UINT16 delay);
void SSerial_recv(void);
void SSerial_tx_pin_write(UINT8 pin_state);
UINT8 SSerial_rx_pin_read();
void SoftwareSerial(UINT8 receivePin, UINT8 transmitPin);
void SSerial_setTx(UINT8 transmitPin);
void SSerial_setRx(UINT8 receivePin);
void SSerial_begin(long speed);
void SSerial_end(void);
int SSerial_read(void);
int SSerial_available(void);
size_t SSerial_write(UINT8 b);
void SSerial_flush(void);
int SSerial_peek(void);
void SSerial_printString(const char *s);
void SSerial_printNumber(long n, UINT8 base);
void SSerial_printFloat(double number, UINT8 point);

#endif	/* SOFTWARESERIAL_H */

