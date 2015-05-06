/*
 * File: G15.c
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on June 20, 2014, 4:27 PM
 */

#include "G15.h"
#include "HardwareSerial.h"

char _G15_CTRL;
char _AX12_CTRL;
char TxRx;

void G15_begin(unsigned long baud, char G15_CTRL, char AX12_CTRL)
{
  Serial_begin(baud);
  Serial_setTimeout(SerialTimeOut);
  _G15_CTRL = G15_CTRL;
  _AX12_CTRL = AX12_CTRL;

  pinMode(_G15_CTRL, OUTPUT);
  pinMode(_AX12_CTRL, OUTPUT);
  
  digitalWrite(_G15_CTRL, TxMode);
  digitalWrite(_AX12_CTRL, TxMode);
}

void setTx(void)
{
  digitalWrite(_G15_CTRL, TxMode);
}

void setRx(void)
{
  digitalWrite(_G15_CTRL, RxMode);
}

WORD send_packet(BYTE ID, char inst, BYTE* data, BYTE param_len)
{
  int i;
  BYTE packet_len = 0;
  BYTE Status[16];
  BYTE TxBuff[16];
  BYTE checksum; //Check Sum = ~ (ID + Length + Instruction + Parameter1 + ... Parameter N)
  WORD error = 0;

  setTx(); //set for transmit mode

  checksum = 0; //clear Checksum val
  TxBuff[0] = 0xFF; //0xFF not included in checksum
  TxBuff[1] = 0xFF;
  TxBuff[2] = ID;
  checksum += TxBuff[2]; //0-254, 0xFE = broadcast id
  TxBuff[3] = param_len + 2;
  checksum += TxBuff[3]; //INSTRUCTION + PARAMETERS( START ADDR + VALUES ) + CHECKSUM                                                                                                          //0xFF and ID not included
  TxBuff[4] = inst;
  checksum += TxBuff[4]; //

  for(i = 0; i < param_len; i++)
  {
    TxBuff[i + 5] = data[i];
    checksum += TxBuff[i + 5];
  }
  TxBuff[i + 5] = ~checksum; //Checksum with Bit Inversion

  packet_len = TxBuff[3] + 4; //# of BYTEs for the whole packet

  for(i = 0; i < packet_len; i++) //sending out the packet
  {
    Serial_write(TxBuff[i]); //uart function must wait for complete transmission
  }
  Serial_flush();

  // we'll only get a reply if it was not broadcast
  if((ID != 0xFE) || (inst == iPING))
  {
    if(inst == iREAD_DATA) //if a read instruction
    {
      param_len = data[1];
      packet_len = data[1] + 6; // data[1] = length of the data to be read
    }
    else
    {
      packet_len = 6; //return status packet always 6 BYTEs
    }

    error = 0; // Clear error
    setRx(); // Set to receive mode
    BYTE readcount = Serial_readBytes(Status, packet_len);

    if(readcount != packet_len)
    {
			error |= 0x0100;
			//return (error);			//packet lost or receive time out
		}
    if((Status[0] != 0xFF) || (Status[1] != 0xFF))
    {
      error |= 0x0200;
      //return (error);			//1000 00001	//wrong header
    }
    if(Status[2] != ID)
    {
      error |= 0x0400;
      //return (error);			//ID mismatch
    }
    if(Status[4] != 0)
    {
      error |= (WORD) Status[4];
      //return(error);
    }

    //calculate checksum
    checksum = 0;
    for(i = 2; i < packet_len; i++) //whole package including checksum but excluding header
    {
      checksum += Status[i]; //correct end result must be 0xFF
    }
    if(checksum != 0xFF)
    {
      error |= 0x0800; //return packet checksum mismatch error
      //return (error);
    }
    if(Status[4] == 0x00 && (error & 0x0100) == 0x00) //copy data only if there is no packet error
    {
      if(inst == iPING)
      {
        // ID is passed to the data[0]
        data[0] = Status[2];
      }
      else if(inst == iREAD_DATA)
      {
        for(i = 0; i < param_len; i++) //Requested Parameters
          data[i] = (BYTE) Status[i + 5];
      }
    }
  }

  return(error); // return error code
}

WORD G15_setWheelMode(BYTE ServoID) //10 bits speed (0-1024)
{
  WORD Error = 0;

  Error = G15_setAngleLimit(ServoID, 0, 0); //enable wheel mode
  if(Error != 0) return(Error);

  Error = G15_setTorqueOnOff(ServoID, 1, iWRITE_DATA); //enable torque

  return(Error);
}

WORD G15_exitWheelMode(BYTE ServoID)
{
  return(G15_setAngleLimit(ServoID, 0, 1087)); //reset to default angle limit
}

WORD G15_setWheelSpeed(BYTE ServoID, WORD Speed, BYTE CW_CCW)
{
  Speed = Speed & 0x03FF; //0000 0011 1111 1111 eliminate bits which are non speed
  if(CW_CCW)
  { //if CW
    Speed = Speed | 0x0400;
  }
  return(G15_setSpeed(ServoID, Speed, iWRITE_DATA));
}

WORD G15_setPos(BYTE ServoID, WORD Position, BYTE Write_Reg)
{
  BYTE TxBuff[3];

  TxBuff[0] = GOAL_POSITION_L; //Control Starting Address
  TxBuff[1] = (BYTE) (Position & 0x00FF); //goal pos bottom 8 bits
  TxBuff[2] = (BYTE) (Position >> 8); //goal pos top 8 bits

  // write the packet, return the error code
  return(send_packet(ServoID, Write_Reg, TxBuff, 3));
}

WORD G15_rotateCW(BYTE ServoID, WORD Position, BYTE Write_Reg)
{
  Position = Position | 0xC000; //directional positioning mode CW

  return(G15_setPos(ServoID, Position, Write_Reg));
}

WORD G15_RotateCCW(BYTE ServoID, WORD Position, BYTE Write_Reg)
{
  Position = Position | 0x8000; //directional positioning mode
  Position = Position & 0xBFFF; //CCW	1011 1111 1111 1111

  return(G15_setPos(ServoID, Position, Write_Reg));
}

WORD G15_setTorqueOnOff(BYTE ServoID, BYTE on_off, BYTE Write_Reg)
{
  BYTE TxBuff[2];

  TxBuff[0] = TORQUE_ENABLE; //Control Starting Address
  TxBuff[1] = on_off; //ON = 1, OFF = 0

  // write the packet, return the error code
  return(send_packet(ServoID, Write_Reg, TxBuff, 2));
}

WORD G15_setSpeed(BYTE ServoID, WORD Speed, BYTE Write_Reg)
{
  BYTE TxBuff[3];

  TxBuff[0] = MOVING_SPEED_L; //Control Starting Address
  TxBuff[1] = (BYTE) (Speed & 0x00FF); //speed bottom 8 bits
  TxBuff[2] = (BYTE) (Speed >> 8); //speed top 8 bits

  // write the packet, return the error code
  return(send_packet(ServoID, Write_Reg, TxBuff, 3));
}

WORD G15_setTimetoGoal(BYTE ServoID, WORD Time, BYTE Write_Reg)
{
  Time = Time & 0x0FFF; //			0000 1111 1111 1111
  Time = Time | 0x8000; //bit 15 represents the time to goal pos mode

  return(G15_setSpeed(ServoID, Time, Write_Reg));
}

WORD G15_setAngleLimit(BYTE ServoID, WORD CW_angle, WORD CCW_angle)
{
  BYTE TxBuff[5];
  WORD error;

  TxBuff[0] = CW_ANGLE_LIMIT_L; //Control Starting Address
  TxBuff[1] = (BYTE) (CW_angle & 0x00FF); //CW limit bottom 8 bits
  TxBuff[2] = (BYTE) (CW_angle >> 8); //CW limit top 8 bits
  TxBuff[3] = (BYTE) (CCW_angle & 0x00FF); //CCW limit bottom 8 bits
  TxBuff[4] = (BYTE) (CCW_angle >> 8); //CCW limit top 8 bits

  error = send_packet(ServoID, iWRITE_DATA, TxBuff, 5);
  delay(10);

  // write the packet, return the error code
  return(error);
}

WORD G15_setTorqueLimit(BYTE ServoID, WORD TorqueLimit)
{
  BYTE TxBuff[3];

  TxBuff[0] = TORQUE_LIMIT_L; //Starting Address
  TxBuff[1] = (BYTE) (TorqueLimit & 0x00FF); //Torque limit bottom 8 bits
  TxBuff[2] = (BYTE) (TorqueLimit >> 8); //Torque limit top 8 bits

  // write the packet, return the error code
  return(send_packet(ServoID, iWRITE_DATA, TxBuff, 3));
}

WORD G15_setTemperatureLimit(BYTE ServoID, BYTE Temperature)
{
  BYTE TxBuff[2];
  WORD error;

  TxBuff[0] = LIMIT_TEMPERATURE; //Starting Address
  TxBuff[1] = Temperature; //temperature

  error = send_packet(ServoID, iWRITE_DATA, TxBuff, 2);
  delay(10);
  // write the packet, return the error code
  return(error);
}

WORD G15_setVoltageLimit(BYTE ServoID, BYTE VoltageLow, BYTE VoltageHigh)
{
  BYTE TxBuff[3];
  WORD error;

  TxBuff[0] = DOWN_LIMIT_VOLTAGE; //Starting Address
  TxBuff[1] = VoltageLow; //lower voltage limit
  TxBuff[2] = VoltageHigh; //Higher voltage limit

  error = send_packet(ServoID, iWRITE_DATA, TxBuff, 3);
  delay(10);
  // write the packet, return the error code
  return(error);
}

WORD G15_setID(BYTE ServoID, BYTE NewID)
{
  BYTE TxBuff[2];
  WORD error;
  TxBuff[0] = ID; // Control Starting Address
  TxBuff[1] = NewID;

  error = send_packet(ServoID, iWRITE_DATA, TxBuff, 2);
  delay(10);
  return(error);
}

WORD G15_setLED(BYTE ServoID, BYTE on_off, BYTE Write_Reg)
{
  BYTE TxBuff[2];

  TxBuff[0] = G15LED; // Control Starting Address
  TxBuff[1] = on_off; // ON = 1, OFF = 0

  // Write the packet, return the error code
  return(send_packet(ServoID, Write_Reg, TxBuff, 2));
}

WORD G15_setAlarmLED(BYTE ServoID, BYTE AlarmLED)
{
  BYTE alarmval = 0x00;
  BYTE TxBuff[2];
  WORD error;

  alarmval = alarmval | AlarmLED;

  TxBuff[0] = ALARM_LED; //Control Starting Address
  TxBuff[1] = alarmval; //alarm val

  error = send_packet(ServoID, iWRITE_DATA, TxBuff, 2);
  delay(10);

  // write the packet, return the error code
  return(error);
}

WORD G15_setAlarmShutDown(BYTE ServoID, BYTE Alarm)
{
  BYTE alarmval = 0x00;
  BYTE TxBuff[2];
  WORD error;

  alarmval = alarmval | Alarm;

  TxBuff[0] = ALARM_SHUTDOWN; //Control Starting Address
  TxBuff[1] = alarmval; //alarm

  error = send_packet(ServoID, iWRITE_DATA, TxBuff, 2);
  delay(10);
  // write the packet, return the error code
  return(error);
}

WORD G15_setMarginSlopePunch(BYTE ServoID, BYTE CWMargin, BYTE CCWMargin, BYTE CWSlope, BYTE CCWSlope, WORD Punch)
{
  BYTE TxBuff[5];
  WORD error = 0;

  TxBuff[0] = CW_COMPLIANCE_MARGIN; //Control Starting Address
  TxBuff[1] = CWMargin;
  TxBuff[2] = CCWMargin;
  TxBuff[3] = CWSlope;
  TxBuff[4] = CCWSlope;

  // write the packet, return the error code
  error = send_packet(ServoID, iWRITE_DATA, TxBuff, 5);

  if(error != 0)
    return(error);

  TxBuff[0] = PUNCH_L; //Control Starting Address
  TxBuff[1] = (BYTE) (Punch & 0x00FF); //punch Lower 8 bits
  TxBuff[2] = (BYTE) (Punch >> 8); //punch Higher 8 bits

  // write the packet, return the error code
  error = send_packet(ServoID, iWRITE_DATA, TxBuff, 3);

  return(error);
}

WORD G15_setBaudRate(BYTE ServoID, long bps)
{
  BYTE TxBuff[2];
  WORD error;

  TxBuff[0] = BAUD_RATE; //Control Starting Address
  TxBuff[1] = (2000000 / bps) - 1; //Calculate baudrate
  //Speed (BPS) = 32M / (16*(n + 1))=2000000/(n+1)

  error = send_packet(ServoID, iWRITE_DATA, TxBuff, 2);
  delay(10);
  // write the packet, return the error code
  return(error);
}

//******************************************************************
//*	RESET TO FACTORY SETTINGS
//* 	eg:	FactoryReset(1,1);// Reset servo 1
//******************************************************************

WORD G15_factoryReset(BYTE ServoID)
{
  BYTE TxBuff[1]; //dummy BYTE
  WORD error;

  error = send_packet(ServoID, iRESET, TxBuff, 0);
  delay(100);

  // write the packet, return the error code
  return(error);
}

WORD G15_ping(BYTE ServoID, BYTE* data)
{
  // write the packet, return the error code
  return(send_packet(ServoID, iPING, data, 0));
}

WORD G15_getPos(BYTE ServoID, BYTE* data)
{
  data[0] = PRESENT_POSITION_L; // Starting Addr where data to be read
  data[1] = 0x02; // # of BYTEs to be read

  return(send_packet(ServoID, iREAD_DATA, data, 2));
}

WORD G15_getSpeed(BYTE ServoID, BYTE* data)
{
  data[0] = PRESENT_SPEED_L; // Starting Addr where data to be read
  data[1] = 0x02; // # of BYTEs to be read

  return(send_packet(ServoID, iREAD_DATA, data, 2));
}

WORD G15_getLoad(BYTE ServoID, BYTE* data)
{
  data[0] = PRESENT_LOAD_L; // Starting Addr where data to be read
  data[1] = 0x02; // # of BYTEs to be read

  return(send_packet(ServoID, iREAD_DATA, data, 2));
}

WORD G15_getVoltage(BYTE ServoID, BYTE* data)
{
  data[0] = PRESENT_VOLTAGE; // Starting Addr where data to be read
  data[1] = 0x01; // # of BYTEs to be read

  return(send_packet(ServoID, iREAD_DATA, data, 2));
}

WORD G15_getTemperature(BYTE ServoID, BYTE* data)
{
  data[0] = PRESENT_TEMPERATURE; // Starting Addr where data to be read
  data[1] = 0x01; // # of BYTEs to be read

  return(send_packet(ServoID, iREAD_DATA, data, 2));
}

WORD G15_getTorqueOnOff(BYTE ServoID, BYTE* data)
{
  data[0] = TORQUE_ENABLE; // Starting Addr where data to be read
  data[1] = 0x01; // # of BYTEs to be read

  return(send_packet(ServoID, iREAD_DATA, data, 2));
}

WORD G15_isMoving(BYTE ServoID, BYTE* data)
{
  data[0] = MOVING; // Starting Addr where data to be read
  data[1] = 0x01; // # of BYTEs to be read

  return(send_packet(ServoID, iREAD_DATA, data, 2));
}

void G15_detAction(void)
{
  BYTE TxBuff[1]; //dummy BYTE

  send_packet(0xFE, iACTION, TxBuff, 0);
}
