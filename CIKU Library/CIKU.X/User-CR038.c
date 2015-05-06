/*
 * File: User-CR038.c
 * Author: Ilyasaa
 * Software: MPLAB X IDE V2.15
 * Compiler: XC8 V1.32
 * Created on August 8, 2014
 *
 * Manufacturer Block : 0
 * Trailer Block: 3,7,11,15,19,23,27,31,35,39,43,47,51,55,59,63
 * Data Block: 1,2,4,5,6,8,9,10,12,13,14,16,17,18,20,21,22,24,
 * 25,26, 28,29,30,32,33,34,36,37,38,40,41,42,44,45,46,48,49,50,
 * 52,53,54,56,57,58,60,61,62
 *
 * Note: Do not write in Manufacturer Block and Trailer Block.
 *       Only write in Data Block.
 */

#include "Arduino.h"
#include "HardwareSerial.h"
#include "LiquidCrystal.h"
#include "CR038.h"

//16Byte data to be stored in the MiFare Card Data Block
unsigned char BLOCK1[16] = "Name:Cytron   01";
unsigned char BLOCK4[16] = "Name:Cytron   04";
unsigned char BLOCK8[16] = "Name:Cytron   08";
unsigned char BLOCK12[16] = "Name:Cytron   12";
unsigned char BLOCK16[16] = "Name:Cytron   16";
unsigned char BLOCK20[16] = "Name:Cytron   20";
unsigned char BLOCK24[16] = "Name:Cytron   24";
unsigned char BLOCK28[16] = "Name:Cytron   28";
unsigned char BLOCK32[16] = "Name:Cytron   32";
unsigned char BLOCK36[16] = "Name:Cytron   36";
unsigned char BLOCK40[16] = "Name:Cytron   40";
unsigned char BLOCK44[16] = "Name:Cytron   44";
unsigned char BLOCK48[16] = "Name:Cytron   48";
unsigned char BLOCK52[16] = "Name:Cytron   52";
unsigned char BLOCK56[16] = "Name:Cytron   56";
unsigned char BLOCK60[16] = "Name:Cytron   60";

#define MREAD    0
#define MWRITE   1

char mode = MREAD;

void setup()
{
  pinMode(LED, OUTPUT); //onboard LED as output
  pinMode(SW, INPUT); //onboard SW as input

  CR038_begin(); //Initialize CR038
  //if succesfully initialize, CR038 onboard Led will blinking 3 times
  CR038_checkError(); //if error in initialize CR038, CIKU onboard LED will blinking 3 times

  lcd_4bit(8, 9, 4, 5, 6, 7); // RS, E, D4, D5, D6, D7
  lcd_begin(16, 2);
  lcd_clear();
}

void loop()
{
  //Select mode using SW
  if(digitalRead(SW) == 0)
  { //SW press
    if(mode == MREAD)
    {
      mode = MWRITE;
      lcd_setCursor(0, 0);
      lcd_printString("Mode: WRITE");
    } else if(mode == MWRITE)
    {
      mode = MREAD;
      lcd_setCursor(0, 0);
      lcd_printString("Mode: READ ");
    }
  }

  //Get NUID of MiFare card subroutine
  CR038_getNuid(); //NUID of MiFare card  is stored in NUID[] array after execution of this subroutine

  if(CR038_checkError() == 0) //NUID of Mifare card is succecfully read
  {
    lcd_setCursor(0, 2);
    for(int i = 3; i >= 0; i--)
    {
      lcd_printNumber(NUID[i], 16);
    }
    lcd_printString("        ");
    delay(500);

    if(mode == MREAD)
    {
      lcd_clear();
      lcd_setCursor(0, 0);
      lcd_printString("Read");
      delay(1000);

      for(int i = 0; i < 16; i++)
      {
        if(i == 0) CR038_read(i + 1);
        else CR038_read(i * 4);

        if(CR038_checkError() != 0)
        {
          lcd_clear();
          lcd_setCursor(0, 0);
          lcd_printString("Error Read");
          delay(1000);
          break;
        }

        lcd_clear();
        lcd_setCursor(0, 0);
        lcd_printString("Block ");
        if(i == 0) lcd_printNumber(i + 1, 10);
        else lcd_printNumber(i * 4, 10);
        delay(500);
        lcd_clear();
        lcd_setCursor(0, 0);
        for(int i = 9; i < 25; i++)
        {
          lcd_write(reply_buffer[i]);
        }
        delay(500);
      }
    } else if(mode == MWRITE)
    {
      lcd_clear();
      lcd_setCursor(0, 0);
      lcd_printString("Write");
      delay(1000);

      for(int i = 0; i < 16; i++)
      {
        lcd_clear();
        lcd_setCursor(0, 0);
        lcd_printString("Block ");
        if(i == 0) lcd_printNumber(i + 1, 10);
        else lcd_printNumber(i * 4, 10);
        delay(100);

        if(i == 0)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK1[j];
          }
        } else if(i == 1)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK4[j];
          }
        } else if(i == 2)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK8[j];
          }
        } else if(i == 3)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK12[j];
          }
        } else if(i == 4)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK16[j];
          }
        } else if(i == 5)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK20[j];
          }
        } else if(i == 6)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK24[j];
          }
        } else if(i == 7)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK28[j];
          }
        } else if(i == 8)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK32[j];
          }
        } else if(i == 9)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK36[j];
          }
        } else if(i == 10)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK40[j];
          }
        } else if(i == 11)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK44[j];
          }
        } else if(i == 12)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK48[j];
          }
        } else if(i == 13)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK52[j];
          }
        } else if(i == 14)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK56[j];
          }
        } else if(i == 15)
        {
          for(int j = 0; j < 16; j++)
          {
            WRITE[9 + j] = BLOCK60[j];
          }
        }

        if(i == 0) CR038_write(i + 1);
        else CR038_write(i * 4);

        if(CR038_checkError() != 0)
        {
          lcd_clear();
          lcd_setCursor(0, 0);
          lcd_printString("Error Write");
          delay(1000);
          break;
        }
      }
    }
    lcd_clear();
    lcd_setCursor(0, 0);
    lcd_printString("Done");
    delay(2000);
    lcd_clear();
  } else
  {
    lcd_setCursor(0, 2);
    lcd_printString("Card Not Found");
  }
}
