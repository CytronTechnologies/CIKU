/* 
 * File: G15.h
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on June 20, 2014, 4:27 PM
 */

#ifndef G15_H
#define	G15_H

#include "Arduino.h"

#define SerialTimeOut 100
#define TxMode LOW
#define RxMode HIGH
#define ConvertAngle2Pos(Angle) (WORD)((WORD)(Angle)*1088UL/360UL)
#define ConvertPos2Angle(Pos) (float)(Pos)*360.0/1088.0
#define ConvertTime(Time) (WORD)(Time*10UL)

#define ON  1
#define OFF 0
#define CW  1
#define CCW 0

// Alarm mask	1111 1111
#define ALARM_INST        0x40
#define ALARM_OVERLOAD    0x20
#define ALARM_CHECKSUM    0x10
#define ALARM_RANGE       0x08
#define ALARM_OVERHEAT    0x04
#define ALARM_ANGLELIMIT 	0x02
#define ALARM_VOLTAGE     0x01

// Instructions
#define iPING       0x01 // Obtain a status packet
#define iREAD_DATA  0x02 // Read Control Table values
#define iWRITE_DATA	0x03 // Write Control Table values
#define iREG_WRITE 	0x04 // Write and wait for ACTION instruction
#define iACTION     0x05 // Triggers REG_WRITE instruction
#define iRESET      0x06 // Set factory defaults
#define iSYNC_WRITE 0x83 // Simultaneously control multiple actuators

enum
{
  MODEL_NUMBER_L, // 0x00
  MODEL_NUMBER_H, // 0x01
  VERSION, // 0x02
  ID, // 0x03
  BAUD_RATE, // 0x04
  RETURN_DELAY_TIME, // 0x05
  CW_ANGLE_LIMIT_L, // 0x06
  CW_ANGLE_LIMIT_H, // 0x07
  CCW_ANGLE_LIMIT_L, // 0x08
  CCW_ANGLE_LIMIT_H, // 0x09
  RESERVED1, // 0x0A
  LIMIT_TEMPERATURE, // 0x0B
  DOWN_LIMIT_VOLTAGE, // 0x0C
  UP_LIMIT_VOLTAGE, // 0x0D
  MAX_TORQUE_L, // 0x0E
  MAX_TORQUE_H, // 0x0F
  STATUS_RETURN_LEVEL, // 0x10
  ALARM_LED, // 0x11
  ALARM_SHUTDOWN, // 0x12
  RESERVED2, // 0x13
  DOWN_CALIBRATION_L, // 0x14
  DOWN_CALIBRATION_H, // 0x15
  UP_CALIBRATION_L, // 0x16
  UP_CALIBRATION_H, // 0x17
  TORQUE_ENABLE, // 0x18
  G15LED, // 0x19
  CW_COMPLIANCE_MARGIN, // 0x1A
  CCW_COMPLIANCE_MARGIN, // 0x1B
  CW_COMPLIANCE_SLOPE, // 0x1C
  CCW_COMPLIANCE_SLOPE, // 0x1D
  GOAL_POSITION_L, // 0x1E
  GOAL_POSITION_H, // 0x1F
  MOVING_SPEED_L, // 0x20
  MOVING_SPEED_H, // 0x21
  TORQUE_LIMIT_L, // 0x22
  TORQUE_LIMIT_H, // 0x23
  PRESENT_POSITION_L, // 0x24
  PRESENT_POSITION_H, // 0x25
  PRESENT_SPEED_L, // 0x26
  PRESENT_SPEED_H, // 0x27
  PRESENT_LOAD_L, // 0x28
  PRESENT_LOAD_H, // 0x29
  PRESENT_VOLTAGE, // 0x2A
  PRESENT_TEMPERATURE, // 0x2B
  REGISTERED_INSTRUCTION, // 0x2C
  RESERVE3, // 0x2D
  MOVING, // 0x2E
  LOCK, // 0x2F
  PUNCH_L, // 0x30
  PUNCH_H // 0x31
};

/***** Function Prototype *****/
void G15_begin(unsigned long baud, char G15_CTRL, char AX12_CTRL);

void setTx(void);
void setRx(void);
WORD send_packet(BYTE ID, char inst, BYTE* data, BYTE param_len);

// Wheel Mode
WORD G15_setWheelMode(BYTE ServoID);
WORD G15_exitWheelMode(BYTE ServoID);
WORD G15_setWheelSpeed(BYTE ServoID, WORD Speed, BYTE CW_CCW);

// Normal Positioning Mode
WORD G15_setPos(BYTE ServoID, WORD Position, BYTE Write_Reg);
WORD G15_rotateCW(BYTE ServoID, WORD Position, BYTE Write_Reg);
WORD G15_rotateCCW(BYTE ServoID, WORD Position, BYTE Write_Reg);
WORD G15_setTorqueOnOff(BYTE ServoID, BYTE on_off, BYTE Write_Reg);
WORD G15_setSpeed(BYTE ServoID, WORD Speed, BYTE Write_Reg);
WORD G15_setTimetoGoal(BYTE ServoID, WORD Time, BYTE Write_Reg);

// Set Maximum Limits
WORD G15_setAngleLimit(BYTE ServoID, WORD CW_angle, WORD CCW_angle);
WORD G15_setTorqueLimit(BYTE ServoID, WORD TorqueLimit);
WORD G15_setTemperatureLimit(BYTE ServoID, BYTE Temperature);
WORD G15_setVoltageLimit(BYTE ServoID, BYTE VoltageLow, BYTE VoltageHigh);

WORD G15_setID(BYTE ServoID, BYTE NewID);
WORD G15_setLED(BYTE ServoID, BYTE on_off, BYTE Write_Reg);
WORD G15_setAlarmLED(BYTE ServoID, BYTE AlarmLED);
WORD G15_setAlarmShutDown(BYTE ServoID, BYTE Alarm);

// Servo Positioning Control Parameters
WORD G15_setMarginSlopePunch(BYTE ServoID, BYTE CWMargin, BYTE CCWMargin, BYTE CWSlope, BYTE CCWSlope, WORD Punch);
WORD G15_setBaudRate(BYTE ServoID, long bps);
WORD G15_factoryReset(BYTE ServoID);
WORD G15_ping(BYTE ServoID, BYTE* data);
WORD G15_getPos(BYTE ServoID, BYTE* data);
WORD G15_getSpeed(BYTE ServoID, BYTE* data);
WORD G15_getLoad(BYTE ServoID, BYTE* data);
WORD G15_getVoltage(BYTE ServoID, BYTE* data);
WORD G15_getTemperature(BYTE ServoID, BYTE* data);
WORD G15_getTorqueOnOff(BYTE ServoID, BYTE* data);
WORD G15_isMoving(BYTE ServoID, BYTE* data);
void G15_setAction(void);

extern char ERR_FLAG;
extern BYTE inByte[10];
extern int byteCount;

#endif	/* G15_H */

