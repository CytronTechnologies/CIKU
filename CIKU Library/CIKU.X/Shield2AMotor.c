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

#include "Shield2AMotor.h"

void Shield2AMotor_begin(BOOL mode, BOOL sensor)
{
  pinMode(DIR1, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(DIR2, OUTPUT);

  digitalWrite(DIR1, LOW);
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);
  digitalWrite(DIR2, LOW);

  _mode = mode;

  if(sensor == LSS05_ENABLE)
  {
    pinMode(senLeft, INPUT);
    pinMode(senMLeft, INPUT);
    pinMode(senMiddle, INPUT);
    pinMode(senMRight, INPUT);
    pinMode(senRight, INPUT);
  }
}

void Shield2AMotor_control(INT16 motor1Speed, INT16 motor2Speed)
{
  if(_mode == SIGNED_MAGNITUDE)
  {
    if(motor1Speed >= 0)
    {
      if(motor1Speed > 100) motor1Speed = 100;
      motor1Speed = motor1Speed * 10;
      analogWrite(EN1, motor1Speed);
      digitalWrite(DIR1, LOW);
    }
    else if(motor1Speed < 0)
    {
      if(motor1Speed < -100) motor1Speed = -100;
      motor1Speed = motor1Speed * -10;
      analogWrite(EN1, motor1Speed);
      digitalWrite(DIR1, HIGH);
    }

    if(motor2Speed >= 0)
    {
      if(motor2Speed > 100) motor2Speed = 100;
      motor2Speed = motor2Speed * 10;
      analogWrite(EN2, motor2Speed);
      digitalWrite(DIR2, LOW);
    }
    else if(motor2Speed < 0)
    {
      if(motor2Speed < -100) motor2Speed = -100;
      motor2Speed = motor2Speed * -10;
      analogWrite(EN2, motor2Speed);
      digitalWrite(DIR2, HIGH);
    }
  }
  else if(_mode == LOCKED_ANTI_PHASE)
  {
    if(motor1Speed >= 0)
    {
      if(motor1Speed > 100) motor1Speed = 100;
      motor1Speed = (motor1Speed * -5) + 511;
      analogWrite(EN1, motor1Speed);
      digitalWrite(DIR1, HIGH);
    }
    else if(motor1Speed < 0)
    {
      if(motor1Speed < -100) motor1Speed = -100;
      motor1Speed = (motor1Speed * -5) + 511;
      analogWrite(EN1, motor1Speed);
      digitalWrite(DIR1, HIGH);
    }

    if(motor2Speed >= 0)
    {
      if(motor2Speed > 100) motor2Speed = 100;
      motor2Speed = (motor2Speed * -5) + 511;
      analogWrite(EN2, motor2Speed);
      digitalWrite(DIR2, HIGH);
    }
    else if(motor2Speed < 0)
    {
      if(motor2Speed < -100) motor2Speed = -100;
      motor2Speed = (motor2Speed * -5) + 511;
      analogWrite(EN2, motor2Speed);
      digitalWrite(DIR2, HIGH);
    }
  }
}

BYTE Shield2AMotor_readLSS05(void)
{
  BYTE data = 0;
  if(digitalRead(senRight) == HIGH) data = data + 0b00001;
  if(digitalRead(senMRight) == HIGH) data = data + 0b00010;
  if(digitalRead(senMiddle) == HIGH) data = data + 0b00100;
  if(digitalRead(senMLeft) == HIGH) data = data + 0b01000;
  if(digitalRead(senLeft) == HIGH) data = data + 0b10000;
  return data;
}

void Shield2AMotor_waveDrive(UINT16 pulse, UINT8 CW_CCW, UINT16 speed_ms)
{
  for(int i = 0; i < pulse; i++)
  {
    switch(position)
    {
      case 1:
        Shield2AMotor_control(100, 0);
        if(CW_CCW == CW) position++;
        else position = 4;
        break;
      case 2:
        Shield2AMotor_control(0, 100);
        if(CW_CCW == CW) position++;
        else position--;
        break;
      case 3:
        Shield2AMotor_control(-100, 0);
        if(CW_CCW == CW) position++;
        else position--;
        break;
      case 4:
        Shield2AMotor_control(0, -100);
        if(CW_CCW == CW) position = 1;
        else position--;
        break;
    }
    delay(speed_ms);
  }
}

void Shield2AMotor_fullDrive(UINT16 pulse, UINT8 CW_CCW, UINT16 speed_ms)
{
  for(int i = 0; i < pulse; i++)
  {
    switch(position)
    {
      case 1:
        Shield2AMotor_control(100, 100);
        if(CW_CCW == CW) position++;
        else position = 4;
        break;
      case 2:
        Shield2AMotor_control(-100, 100);
        if(CW_CCW == CW) position++;
        else position--;
        break;
      case 3:
        Shield2AMotor_control(-100, -100);
        if(CW_CCW == CW) position++;
        else position--;
        break;
      case 4:
        Shield2AMotor_control(100, -100);
        if(CW_CCW == CW) position = 1;
        else position--;
        break;
    }
    delay(speed_ms);
  }
}

void Shield2AMotor_halfStepDrive(UINT16 pulse, UINT8 CW_CCW, UINT16 speed_ms)
{
  for(int i = 0; i < pulse; i++)
  {
    switch(position)
    {
      case 1:
        Shield2AMotor_control(100, 0);
        if(CW_CCW == CW) position++;
        else position = 8;
        break;
      case 2:
        Shield2AMotor_control(100, 100);
        if(CW_CCW == CW) position++;
        else position--;
        break;
      case 3:
        Shield2AMotor_control(0, 100);
        if(CW_CCW == CW) position++;
        else position--;
        break;
      case 4:
        Shield2AMotor_control(-100, 100);
        if(CW_CCW == CW) position++;
        else position--;
        break;
      case 5:
        Shield2AMotor_control(-100, 0);
        if(CW_CCW == CW) position++;
        else position--;
        break;
      case 6:
        Shield2AMotor_control(-100, -100);
        if(CW_CCW == CW) position++;
        else position--;
        break;
      case 7:
        Shield2AMotor_control(0, -100);
        if(CW_CCW == CW) position++;
        else position--;
        break;
      case 8:
        Shield2AMotor_control(100, -100);
        if(CW_CCW == CW) position = 1;
        else position--;
        break;
    }
    delay(speed_ms);
  }
}
