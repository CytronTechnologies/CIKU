/*
 * File: PS2.c
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on June 25, 2014, 12:20 PM
 */

#include "PS2.h"

void PS2_begin(unsigned long baud, char rx, char tx)
{
  if(rx == 0 && tx == 1)
	{
    HW = true;
		Serial_begin(baud);
	}

	else if(rx == 2 && tx == 3)
	{
		HW = false;
		pinMode(rx,INPUT);
		pinMode(tx,OUTPUT);
		SSerial_begin(baud);
	}
	pinMode(PS2_RESET, OUTPUT);
	digitalWrite(PS2_RESET, HIGH);
}

void PS2_write(BYTE d)
{
  if(HW)
  {
    while(Serial_available())
      Serial_read();
    Serial_write(d);
    Serial_flush();
  }
  else if(!HW)
  {
    while(SSerial_available())
      SSerial_read();
    SSerial_write(d);
  }
}

BYTE PS2_read(void)
{
  BYTE rec_data;
	long waitcount = 0;

	if(HW)
	{
		while(1)
		{
			if(Serial_available() > 0) // Read the incoming byte
      {
				rec_data = Serial_read();
				SERIAL_ERR = false;
        break;
			}
			waitcount++;
			if(waitcount > 50000)
			{
				SERIAL_ERR = true;
				rec_data = 0xFF;
        break;
			}
		}
	}

	else if(!HW)
	{
		while(1)
		{
			if(SSerial_available() > 0)
			{
				rec_data = SSerial_read();
				SERIAL_ERR = false;
        break;
			}
			waitcount++;
			if(waitcount > 50000)
			{
				SERIAL_ERR = true;
				rec_data = 0xFF;
        break;
			}
		}
	}

  return rec_data;
}

BYTE PS2_getVal(BYTE key)
{
  PS2_write(key);
  return PS2_read();
}

int PS2_getAll(void)
{
  BYTE nbyte;
	long waitcount;

	PS2_write(p_btn_joy);

	if(HW)
	{
		nbyte = Serial_readBytes(ps_data, 6);
		if(nbyte == 6) return 1;
		else return 0;
	}

	else
	{
		waitcount = 0;
		while(SSerial_available() < 6)
		{
			waitcount++;
			if(waitcount > 50000) return 0;
		}
		for(int i = 0; i < 6; i++)
		{
			ps_data[i] = SSerial_read();
		}

		return 1;
	}
}

void PS2_vibrate(BYTE motor, BYTE val)
{
  BYTE m;
	if(motor == 1) m = 29;
	else m = 30;

	PS2_write(m);
	PS2_write(val);
}

void PS2_reset(BYTE reset)
{
  if(reset) digitalWrite(PS2_RESET, LOW);
	else digitalWrite(PS2_RESET, HIGH);
}
