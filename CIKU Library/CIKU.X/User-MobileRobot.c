#include "Arduino.h"
#include "HardwareSerial.h"
#include "pitches.h"

#define DIR1 4
#define EN1  5
#define EN2  6
#define DIR2 7

#define senLeft   A1
#define senMLeft  A2
#define senMiddle A3
#define senMRight A4
#define senRight  A5

#define sw     A0
#define buzzer 9
#define btConnect 10

void motor(signed int leftSpeed, signed int rightSpeed);
void playTone(int *melody, int *duration, int length, int pause);

int welcomeNote[] = {
  NOTE_G4,
  NOTE_C5};
int welcomeDuration[] = {8, 8};

int goNote[] = {
  NOTE_C4,
  NOTE_D4,
  NOTE_E4,
  NOTE_G4,
  0,
  NOTE_E4,
  NOTE_G4};
int goDuration[] = {8, 8, 8, 8, 8, 8, 4};

int connectNote[] = {
  NOTE_C5,
  NOTE_D5,
  NOTE_E5,
  NOTE_F5};
int connectDuration[] = {8, 8, 8, 4};

int disconnectNote[] = {
  NOTE_F5,
  NOTE_E5,
  NOTE_D5,
  NOTE_C5};
int disconnectDuration[] = {8, 8, 8, 4};

unsigned char inByte;
BOOL btFlag = false;

void setup()
{
  Serial_begin(9600);

  pinMode(DIR1, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(DIR2, OUTPUT);

  pinMode(senLeft, INPUT);
  pinMode(senMLeft, INPUT);
  pinMode(senMiddle, INPUT);
  pinMode(senMRight, INPUT);
  pinMode(senRight, INPUT);

  pinMode(sw, INPUT);
  pinMode(buzzer, OUTPUT);

  delay(1000);
  playTone(welcomeNote, welcomeDuration, 2, 0);
  delay(1000);
}

void loop()
{
  if(digitalRead(btConnect) == LOW && btFlag)
  {
    playTone(disconnectNote, disconnectDuration, 4, 0);
    btFlag = !btFlag;
  }

  if(Serial_available())
  {
    inByte = Serial_read();
    
    if(digitalRead(btConnect) == HIGH && !btFlag)
    {
      playTone(connectNote, connectDuration, 4, 0);
      btFlag = !btFlag;
    }
  }
  if(inByte == '0') motor(0, 0);
  else if(inByte == '1') motor(80, 80);
  else if(inByte == '2') motor(-80, -80);
  else if(inByte == '3') motor(-80, 80);
  else if(inByte == '4') motor(80, -80);

  if(!digitalRead(sw))
  {
    playTone(goNote, goDuration, 7, 0);
    delay(1000);
    while(1)
    {
      if(!digitalRead(senLeft) &&
         !digitalRead(senMLeft) &&
         digitalRead(senMiddle) &&
         !digitalRead(senMRight) &&
         !digitalRead(senRight))
      {
        motor(80, 80);
      }
      else if(!digitalRead(senLeft) &&
         digitalRead(senMLeft) &&
         digitalRead(senMiddle) &&
         !digitalRead(senMRight) &&
         !digitalRead(senRight))
      {
        motor(50, 80);
      }
      else if(!digitalRead(senLeft) &&
         !digitalRead(senMLeft) &&
         digitalRead(senMiddle) &&
         digitalRead(senMRight) &&
         !digitalRead(senRight))
      {
        motor(80, 50);
      }
      else if(!digitalRead(senLeft) &&
         digitalRead(senMLeft) &&
         !digitalRead(senMiddle) &&
         !digitalRead(senMRight) &&
         !digitalRead(senRight))
      {
        motor(30, 80);
      }
      else if(!digitalRead(senLeft) &&
         !digitalRead(senMLeft) &&
         !digitalRead(senMiddle) &&
         digitalRead(senMRight) &&
         !digitalRead(senRight))
      {
        motor(80, 30);
      }
      else if(digitalRead(senLeft) &&
         digitalRead(senMLeft) &&
         !digitalRead(senMiddle) &&
         !digitalRead(senMRight) &&
         !digitalRead(senRight))
      {
        motor(10, 80);
      }
      else if(!digitalRead(senLeft) &&
         !digitalRead(senMLeft) &&
         !digitalRead(senMiddle) &&
         digitalRead(senMRight) &&
         digitalRead(senRight))
      {
        motor(80, 10);
      }
      else if(digitalRead(senLeft) &&
         !digitalRead(senMLeft) &&
         !digitalRead(senMiddle) &&
         !digitalRead(senMRight) &&
         !digitalRead(senRight))
      {
        motor(0, 80);
      }
      else if(!digitalRead(senLeft) &&
         !digitalRead(senMLeft) &&
         !digitalRead(senMiddle) &&
         !digitalRead(senMRight) &&
         digitalRead(senRight))
      {
        motor(80, 0);
      }
    }
  }
}

void motor(signed int leftSpeed, signed int rightSpeed)
{
  if(rightSpeed >= 0)
  {
    if(rightSpeed > 100) rightSpeed = 100;
    rightSpeed = rightSpeed * 10;
    analogWrite(EN1, rightSpeed);
    digitalWrite(DIR1, LOW);
  }
  else if(rightSpeed < 0)
  {
    if(rightSpeed < -100) rightSpeed = -100;
    rightSpeed = rightSpeed * -10;
    analogWrite(EN1, rightSpeed);
    digitalWrite(DIR1, HIGH);
  }

  if(leftSpeed >= 0)
  {
    if(leftSpeed > 100) leftSpeed = 100;
    leftSpeed = leftSpeed * 10;
    analogWrite(EN2, leftSpeed);
    digitalWrite(DIR2, LOW);
  }
  else if(leftSpeed < 0)
  {
    if(leftSpeed < -100) leftSpeed = -100;
    leftSpeed = leftSpeed * -10;
    analogWrite(EN2, leftSpeed);
    digitalWrite(DIR2, HIGH);
  }
}

void playTone(int *melody, int *duration, int length, int pause)
{
  for(int i = 0; i < length; i++)
  {
    int noteDuration = 1000 / duration[i];
    tone(buzzer, melody[i], noteDuration);
    int pauseBetweenNotes = noteDuration * (1 + pause);
    delay(pauseBetweenNotes);
    noTone(buzzer);
  }
}
