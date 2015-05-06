/* 
 * File: PS2.h
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on June 25, 2014, 12:20 PM
 */

#ifndef PS2_H
#define	PS2_H

#include "Arduino.h"
#include "HardwareSerial.h"
#include "SoftwareSerial.h"

#define p_select 0
#define p_joyl 	 1
#define p_joyr 	 2
#define p_start  3
#define p_up	 4
#define p_right  5
#define p_down	 6
#define p_left	 7
#define p_l2	   8
#define p_r2	   9
#define p_l1	   10
#define p_r1	   11
#define p_triangle	12
#define p_circle	13
#define p_cross		14
#define p_square	15
#define p_joy_lx	16
#define p_joy_ly	17
#define p_joy_rx	18
#define p_joy_ry	19
#define p_joy_lu	20
#define p_joy_ld	21
#define p_joy_1l	22
#define p_joy_lr	23
#define p_joy_ru	24
#define p_joy_rd	25
#define p_joy_rl	26
#define p_joy_rr	27
#define p_con_status	28
#define p_motor1	29
#define p_motor2	30
#define p_btn_joy	31

#define PS2_RESET A1

#define PRESS   0
#define RELEASE 1

char HW;
char SERIAL_ERR;
char ps_data[6];

void PS2_begin(unsigned long baud, char rx, char tx);
void PS2_write(BYTE d);
BYTE PS2_read(void);
BYTE PS2_getVal(BYTE key);
int PS2_getAll(void);
void PS2_vibrate(BYTE motor, BYTE val);
void PS2_reset(BYTE reset);

#endif	/* PS2_H */
