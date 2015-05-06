/* 
 * File: LiquidCrystal.h
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on June 12, 2014, 11:35 AM
 */

#ifndef LIQUIDCRYSTAL_H
#define	LIQUIDCRYSTAL_H

#include "Arduino.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

UINT8 _displayfunction;
UINT8 _displaycontrol;
UINT8 _displaymode;

UINT8 _rs_pin;
UINT8 _enable_pin;
UINT8 _data_pins[8];
UINT8 _numlines;

void lcd_4bit(UINT8 rs, UINT8 enable,
              UINT8 d0, UINT8 d1, UINT8 d2, UINT8 d3);
void lcd_8bit(UINT8 rs, UINT8 enable,
              UINT8 d0, UINT8 d1, UINT8 d2, UINT8 d3,
              UINT8 d4, UINT8 d5, UINT8 d6, UINT8 d7);
void lcd_begin(UINT8 cols, UINT lines);

void lcd_clear(void);
void lcd_home(void);
void lcd_setCursor(UINT8 col, UINT row);
void lcd_noDisplay(void);
void lcd_display(void);
void lcd_noCursor(void);
void lcd_cursor(void);
void lcd_noBlink(void);
void lcd_blink(void);
void lcd_scrollDisplayLeft(void);
void lcd_scrollDisplayRight(void);
void lcd_leftToRight(void);
void lcd_rightToLeft(void);
void lcd_autoScroll(void);
void lcd_noAutoScroll(void);

void lcd_createChar(UINT8 location, UINT8 charmap[]);

void lcd_command(UINT8 value);
void lcd_write(UINT8 value);
void lcd_send(UINT8 value, UINT8 mode);
void lcd_write4bits(UINT8 value);
void lcd_write8bits(UINT8 value);
void lcd_pulseEnable(void);

void lcd_printString(const char *s);
void lcd_printNumber(long n, UINT8 base);
void lcd_printFloat(double number, UINT8 digit);

#endif	/* LIQUIDCRYSTAL_H */

