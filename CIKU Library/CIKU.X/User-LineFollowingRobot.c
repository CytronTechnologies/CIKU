
#include "Arduino.h"
#include "Shield2AMotor.h"

void setup()
{
  Shield2AMotor_begin(SIGNED_MAGNITUDE, LSS05_ENABLE);
  pinMode(SW, INPUT); // SW on CIKU board
  pinMode(LED, OUTPUT); // LED on CIKU board

  while(digitalRead(SW) == HIGH); // Wait SW button to be pressed
  digitalWrite(LED, HIGH); // Turn ON LED
}

void loop()
{
  BYTE lineSensor = Shield2AMotor_readLSS05();

  if(lineSensor == 0b00100)
  {
    Shield2AMotor_control(80, 80); // Robot move straight forward
  }
  else if(lineSensor == 0b01100)
  {
    Shield2AMotor_control(50, 80); // Left motor speed = 50%, right motor speed = 80%
  }
  else if(lineSensor == 0b00110)
  {
    Shield2AMotor_control(80, 50); // Left motor speed = 80%, right motor speed = 50%
  }
  else if(lineSensor == 0b01000)
  {
    Shield2AMotor_control(30, 80); // Left motor speed = 30%, right motor speed = 80%
  }
  else if(lineSensor == 0b00010)
  {
    Shield2AMotor_control(80, 30); // Left motor speed = 80%, right motor speed = 30%
  }
  else if(lineSensor == 0b11000)
  {
    Shield2AMotor_control(10, 80); // Left motor speed = 10%, right motor speed = 80%
  }
  else if(lineSensor == 0b00011)
  {
    Shield2AMotor_control(80, 10); // Left motor speed = 80%, right motor speed = 10%
  }
  else if(lineSensor == 0b10000)
  {
    Shield2AMotor_control(0, 80); // Left motor stop, right motor speed = 80%
  }
  else if(lineSensor == 0b00001)
  {
    Shield2AMotor_control(80, 0); // Left motor speed = 80%, right motor stop
  }
}
