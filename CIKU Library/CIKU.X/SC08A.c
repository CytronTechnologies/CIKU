/*
 * File: SC08A.c
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on July 7, 2014, 9:20 AM
 */

#include "SC08A.h"

void SC08A_begin(void)
{
  Serial_begin(9600);
  Serial_setTimeout(SerialTimeOut);
//  SC08A_enable(0, 1); // Enable all servo
}

void SC08A_initPos(UINT8 channel, UINT16 pos)
{
  UINT8 byte1 = 0, byteH = 0, byteL = 0;

  byte1 = 0b10000000 | channel;
  byteH = (pos >> 6) & 0b01111111;
  byteL = pos & 0b00111111;

  Serial_write(byte1);
  Serial_write(byteH);
  Serial_write(byteL);
  Serial_flush();
}

void SC08A_enable(UINT8 channel, UINT8 enable)
{
  UINT8 byte1 = 0;

  byte1 = 0b11000000 | channel;

  Serial_write(byte1);
  Serial_write(enable);
  Serial_flush();
}

void SC08A_pos(UINT8 channel, UINT16 pos, UINT8 speed)
{
  UINT8 byte1 = 0, byteH = 0, byteL = 0;

  byte1 = 0b11100000 | channel;
  byteH = (pos >> 6) & 0b01111111;
  byteL = pos & 0b00111111;

  Serial_write(byte1);
  Serial_write(byteH);
  Serial_write(byteL);
  Serial_write(speed);
  Serial_flush();
}

int SC08A_getPos(UINT8 channel)
{
  UINT8 byte1 = 0, pos[2] = {0};

  byte1 = 0b10100000 | channel;
  Serial_write(byte1);
  Serial_flush();

  Serial_readBytes(pos, 2); // Receive 2 bytes

  UINT16 position = pos[0] << 6;
  position = position | (pos[1] & 0x3F);

  return position;
}

void SC08A_end(void)
{
  Serial_end();
}
