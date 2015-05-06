/*
 * Software: MPLAB X IDE V2.15
 * Compiler: XC8 V1.32
 */

#include "Arduino.h"
#include "Shield2AMotor.h"

void setup()
{
  pinMode(LED, OUTPUT); //onboard LED as output
  pinMode(SW, INPUT); //onboard SW as input

  Shield2AMotor_begin(STEPPER, LSS05_DISABLE);
  delay(3000);
}

void loop()
{
  Shield2AMotor_waveDrive(200, CCW, 10); //360 degree rotation using 1.8° stepper motor
  delay(1000);
  Shield2AMotor_waveDrive(200, CW, 10); //360 degree rotation using 1.8° stepper motor
  delay(1000);

  Shield2AMotor_fullDrive(200, CCW, 10); //360 degree rotation using 1.8° stepper motor
  delay(1000);
  Shield2AMotor_fullDrive(200, CW, 10); //360 degree rotation using 1.8° stepper motor
  delay(1000);

  Shield2AMotor_halfStepDrive(400, CCW, 10); //360 degree rotation using 1.8° stepper motor
  delay(1000);
  Shield2AMotor_halfStepDrive(400, CW, 10); //360 degree rotation using 1.8° stepper motor
  delay(1000);
}