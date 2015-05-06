/* 
 * File: Shield2AMotor.h
 * Author: Ilyasaa
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on August 29, 2014, 11:58 AM
 *
 * Modify: Idris
 * - Add sensors function
 */

#ifndef SHIELD2AMOTOR_H
#define	SHIELD2AMOTOR_H

#include "Arduino.h"

#define DIR1 4
#define EN1  5
#define EN2  6
#define DIR2 7
#define CW   0
#define CCW  1
#define SIGNED_MAGNITUDE  false
#define LOCKED_ANTI_PHASE true
#define STEPPER 2

#define senLeft   A1 // Sensor Left
#define senMLeft  A2 // Sensor Middle Left
#define senMiddle A3 // Sensor Middle
#define senMRight A4 // Sensor Middle Right
#define senRight  A5 // Sensor Right
#define LSS05_DISABLE  false
#define LSS05_ENABLE   true

BOOL _mode;
UINT8 position = 1;

void Shield2AMotor_begin(BOOL, BOOL);
void Shield2AMotor_control(INT16, INT16);
BYTE Shield2AMotor_readLSS05(void);
void Shield2AMotor_waveDrive(UINT16, UINT8, UINT16);
void Shield2AMotor_fullDrive(UINT16, UINT8, UINT16);
void Shield2AMotor_halfStepDrive(UINT16, UINT8, UINT16);

#endif	/* SHIELD2AMOTOR_H */

