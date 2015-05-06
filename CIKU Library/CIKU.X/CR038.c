/*
 * File: CR038.c
 * Author: Ilyasaa
 * Software: MPLAB X IDE V2.15
 * Compiler: XC8 V1.32
 * Created on August 8, 2014
 */

#include "Arduino.h"
#include "HardwareSerial.h"
#include "CR038.h"

void CR038_begin(void)
{
  delay(500); //Waiting CR038 to ON before begin "Serial_begin(19200);"
  Serial_begin(19200); //CR038 default baurate is 19200
  Serial_setTimeout(SerialTimeOut);

  for(int i = 0; i < 3; i++)
  { //If connected, CR038 will blinking 3 times
    delay(100);
    CR038_ledOn();
    delay(100);
    CR038_ledOff();
  }
}

void CR038_ledOn(void)
{
  for(int i = 0; i < 10; i++)
  {
    Serial_write(LED_ON[i]);
  }
  CR038_serialRead(10);
}

void CR038_ledOff(void)
{
  for(int i = 0; i < 10; i++)
  {
    Serial_write(LED_OFF[i]);
  }
  CR038_serialRead(10);
}

void CR038_readDev(void)
{
  for(int i = 0; i < 9; i++)
  {
    Serial_write(READ_DEV[i]);
  }
  CR038_serialRead(12);
}

void CR038_authenSec(char block)
{
  AUTHEN_SEC[9] = block;

  unsigned char xor_temp = 0;
  for(int i = 0; i < 12; i++)
  {
    xor_temp ^= AUTHEN_SEC[4 + i];
  } //AUTHEN_SEC, Calculate the checksum
  AUTHEN_SEC[4 + 12] = xor_temp;

  for(int i = 0; i < 17; i++)
  {
    Serial_write(AUTHEN_SEC[i]);
  }
  CR038_serialRead(10);
}

void CR038_read(char block)
{
  CR038_getNuid();
  CR038_cardSel();
  CR038_authenSec(block); //Authenticate in memory of MiFare card

  READ[8] = block;

  unsigned char xor_temp = 0;
  for(int i = 0; i < 5; i++)
  {
    xor_temp ^= READ[4 + i];
  } //READ, Calculate the checksum
  READ[4 + 5] = xor_temp;

  for(int i = 0; i < 10; i++)
  {
    Serial_write(READ[i]);
  }
  CR038_serialRead(26);

  //if NO packet from CR08, NUID = 0xFFFFFFFF
  if(NUID[0] == 0xFF &&
     NUID[1] == 0xFF &&
     NUID[2] == 0xFF &&
     NUID[3] == 0xFF)
  {
    for(int i = 0; i < 26; i++)
    {
      reply_buffer[i] = 0xFF;
    }
  }
}

void CR038_write(char block)
{
  CR038_getNuid();
  CR038_cardSel(); //Select MiFare card
  CR038_authenSec(block); //Authenticate in memory of MiFare card

  WRITE[8] = block;

  unsigned char xor_temp = 0;
  for(int i = 0; i < 21; i++) //WRITE, Calculate the checksum
  {
    xor_temp ^= WRITE[4 + i];
  }
  WRITE[4 + 21] = xor_temp;

  for(int i = 0; i < 26; i++)
  {
    Serial_write(WRITE[i]);
  }
  CR038_serialRead(10);
}

void CR038_getNuid(void)
{
  CR038_antennaOn(); //On the MiFare module onboard antenna
  CR038_cardReq(); //Request card type of MiFare card
  CR038_antiCol(); //Perform anti-collision

  for(int i = 0; i < 4; i++) //Fill up byte 8 to byte 11 of CARD_SEL[] with NUID
  {
    CARD_SEL[i + 8] = NUID[i];
  }

  unsigned char xor_temp = 0;
  for(int i = 0; i < 8; i++) //Calculate the checksum of byte 4 to byte 11 of the CARD_SEL[]
  {
    xor_temp ^= CARD_SEL[4 + i];
  }
  CARD_SEL[12] = xor_temp; //Fill the checksum in byte 12 of CARD_SEL[]
}

int CR038_checkError(void) //LED on CIKU will blinking, return 0 if no errror
{
  if(reply_buffer[8] == 0x00);
  else
  {
    for(int i = 0; i < 3; i++)
    {
      delay(100);
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
    }
  }
  return reply_buffer[8];
}

void CR038_antennaOn(void)
{
  for(int i = 0; i < 10; i++)
  {
    Serial_write(ANTENNA_ON[i]);
  }
  CR038_serialRead(10);
}

void CR038_cardReq(void)
{
  for(int i = 0; i < 10; i++)
  {
    Serial_write(CARD_REQ[i]);
  }
  CR038_serialRead(12);
}

void CR038_antiCol(void)
{
  for(int i = 0; i < 9; i++)
  {
    Serial_write(ANTI_COL[i]);
  }
  int j = 0;
  for(int i = 0; i < 14; i++)
  {
    while(!Serial_available())
    {
      if(j == 100) break; //If waiting RX too long (more than 100ms), break the loop
      delay(1);
      j++;
    }
    reply_buffer[i] = Serial_read();
  }

  //Byte 9 to byte 12 of replied bytes contains NUID of the MiFare card
  //Store them in NUID[] array
  if(j == 100)
  {
    for(int i = 0; i < 4; i++)
    {
      NUID[i] = 0xFF;
    }
  }//if NO packet from CR08, NUID = 0xFFFFFFFF
  else //if there is replied packet from CR08, NUID = NUID of the MiFare card
  {
    for(int i = 0; i < 4; i++)
    {
      NUID[i] = reply_buffer[9 + i];
    }
    delay(500); //If CR038 read a card, CR038 LED will blink
    CR038_ledOn();
    delay(500);
    CR038_ledOff();
  }
}

void CR038_cardSel(void)
{
  for(int i = 0; i < 13; i++)
  {
    Serial_write(CARD_SEL[i]);
  }
  CR038_serialRead(11);
}

void CR038_serialRead(char length)
{
  int j = 0;
  for(int i = 0; i < length; i++)
  {
    while(!Serial_available())
    {
      if(j == 100) break; //If waiting RX too long (more than 100ms), break the loop
      delay(1);
      j++;
    }
    reply_buffer[i] = Serial_read();
  }
}
