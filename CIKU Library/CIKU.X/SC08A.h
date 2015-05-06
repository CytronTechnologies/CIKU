/*
 * File: SC08A.h
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on July 7, 2014, 9:20 AM
 *
 * Note:
 * - If want to use initPos function, need to call it after begin.
 * - enable function need to call to activate servo before control it.
 */

#ifndef SC08A_H
#define	SC08A_H

#include "Arduino.h"
#include "HardwareSerial.h"

#define ON  1
#define OFF 0
#define ALL_SERVO 0

#define SerialTimeOut 1000

void SC08A_begin(void);
void SC08A_initPos(UINT8 channel, UINT16 pos);
void SC08A_enable(UINT8 channel, UINT8 enable);
void SC08A_pos(UINT8 channel, UINT16 pos, UINT8 speed);
int SC08A_getPos(UINT8 channel);
void SC08A_end(void);

#endif	/* SC08A_H */

