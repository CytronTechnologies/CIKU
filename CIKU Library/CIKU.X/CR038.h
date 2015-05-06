/*
 * File: CR038.h
 * Author: Ilyasaa
 * Software: MPLAB X IDE V2.15
 * Compiler: XC8 V1.32
 * Created on August 8, 2014
 *
 * Manufacturer Block : 0
 * Trailer Block : 3,7,11,15,19,23,27,31,35,39,43,47,51,55,59,63
 * Data Block : 1,2,4,5,6,8,9,10,12,13,14,16,17,18,20,21,22,24,
 * 25,26, 28,29,30,32,33,34,36,37,38,40,41,42,44,45,46,48,49,50,
 * 52,53,54,56,57,58,60,61,62
 *
 * Note : Do not write in Manufacturer Block and Trailer Block.
 * only write in Data Block
 */

#ifndef SC08A_H
#define	SC08A_H

#include "Arduino.h"
#include "HardwareSerial.h"

#define SerialTimeOut 1000

/*****MiFare commands*****/
//{ Header,Header,Length,Length,NodeID,NodeID,Command,Command,Parameter.... , XOR }
char LED_ON[] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x07, 0x01, 0x01, 0x07}; //Command to turn on MiFare module onboard LED
char LED_OFF[] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x07, 0x01, 0x00, 0x06}; //Command to turn off MiFare module onboard LED
char ANTENNA_ON[] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x0C, 0x01, 0x01, 0x0C}; //Command to turn off MiFare module onboard antenna
char READ_DEV[] = {0xAA, 0xBB, 0x05, 0x00, 0x00, 0x00, 0x03, 0x01, 0x02}; //Command to read MiFare module device number
char CARD_REQ[] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x01, 0x02, 0x52, 0x51}; //Command to request card type of MiFare card
char ANTI_COL[] = {0xAA, 0xBB, 0x05, 0x00, 0x00, 0x00, 0x02, 02, 00}; //Command for anti-collision
char CARD_SEL[] = {0xAA, 0xBB, 0x09, 0x00, 0x00, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00}; //Command to select specific MiFare card for access

char AUTHEN_SEC[] = {0xAA, 0xBB, 0x0D, 0x00, 0x00, 0x00, 0x07, 0x02, 0x60, /*Block*/0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*XOR*/0x00}; //Command to authenticate Sector 0 in memory of MiFare card
char READ[] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x08, 0x02, /*Block*/0, /*XOR*/0x00}; //Command to read Block 0 in memory of MiFare card
char WRITE[] = {0xAA, 0xBB, 0x16, 0x00, 0x00, 0x00, 0x09, 0x02, /*Block*/1, 0x7A, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, /*XOR*/0x00}; //Command to write data into Block 4 (in Sector 1) in memory of MiFare card
/*****************************/

unsigned char reply_buffer[26]; //Buffer to store data received from MiFare module when a certain command is sent (maximum is 26 bytes in this case)
unsigned char NUID[4]; //Array for storing NUID of a MiFare card

//Public - To be use by normal user
void CR038_begin(void);
void CR038_ledOn(void);
void CR038_ledOff(void);
void CR038_readDev(void); //Read MiFare device number subroutine
void CR038_read(char block); //Read data from Block 0 (manufaturer block) in memory of MiFare card subroutine
void CR038_write(char block); //Write data into from Block 4 in memory of MiFare card subroutine
void CR038_getNuid(void); //Get NUID of MiFare card subroutine
int CR038_checkError(void); //LED on CIKU will blinking

//Private - To be use by advance user
void CR038_authenSec(char block); //Authenticate Sector in MiFare card subroutine
void CR038_antennaOn(void); //Turn on antenna of MiFare module subroutine
void CR038_cardReq(void); //Request MiFare card type subroutine
void CR038_antiCol(void); //Perform Anti-Collison subroutine
void CR038_cardSel(void); //Select MiFare card subroutine
void CR038_serialRead(char length);

#endif	/* CR038_H */