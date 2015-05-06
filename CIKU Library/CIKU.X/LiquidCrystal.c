/*
 * File: LiquidCrystal.h
 * Author: Idris
 * Software: MPLAB X IDE V2.10
 * Compiler: XC8 V1.32
 * Created on June 12, 2014, 11:52 AM
 */

#include "LiquidCrystal.h"

void lcd_4bit(UINT8 rs, UINT8 enable,
              UINT8 d0, UINT8 d1, UINT8 d2, UINT8 d3)
{
  _rs_pin = rs;
  _enable_pin = enable;

  _data_pins[0] = d0;
  _data_pins[1] = d1;
  _data_pins[2] = d2;
  _data_pins[3] = d3;

  pinMode(rs, OUTPUT);
  pinMode(enable, OUTPUT);

  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

void lcd_8bit(UINT8 rs, UINT8 enable,
              UINT8 d0, UINT8 d1, UINT8 d2, UINT8 d3,
              UINT8 d4, UINT8 d5, UINT8 d6, UINT8 d7)
{
  _rs_pin = rs;
  _enable_pin = enable;

  _data_pins[0] = d0;
  _data_pins[1] = d1;
  _data_pins[2] = d2;
  _data_pins[3] = d3;
  _data_pins[4] = d4;
  _data_pins[5] = d5;
  _data_pins[6] = d6;
  _data_pins[7] = d7;

  pinMode(rs, OUTPUT);
  pinMode(enable, OUTPUT);

  _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

void lcd_begin(UINT8 cols, UINT lines)
{
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delay(50);
  // Now we pull both RS and R/W low to begin commands
  digitalWrite(_rs_pin, LOW);
  digitalWrite(_enable_pin, LOW);

  //put the LCD into 4 bit or 8 bit mode
  if (!(_displayfunction & LCD_8BITMODE))
  {
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    lcd_write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    lcd_write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // third go!
    lcd_write4bits(0x03);
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    lcd_write4bits(0x02);
  }
  else
  {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    lcd_command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    lcd_command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(150);

    // third go
    lcd_command(LCD_FUNCTIONSET | _displayfunction);
  }

  // finally, set # lines, font size, etc.
  lcd_command(LCD_FUNCTIONSET | _displayfunction);

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  lcd_display();

  // clear it off
  lcd_clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  lcd_command(LCD_ENTRYMODESET | _displaymode);
}

void lcd_clear(void)
{
  lcd_command(LCD_CLEARDISPLAY);  // Clear display, set cursor position to zero
  delay(2);  // This command takes a long time!
}

void lcd_home(void)
{
  lcd_command(LCD_RETURNHOME);  // Set cursor position to zero
  delay(2);  // This command takes a long time!
}

void lcd_setCursor(UINT8 col, UINT row)
{
  int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  if(row >= _numlines)
  {
    row = _numlines-1;    // We count rows starting w/0
  }
  lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_noDisplay(void)
{
  _displaycontrol &= ~LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_display(void)
{
  _displaycontrol |= LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_noCursor(void)
{
  _displaycontrol &= ~LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_cursor(void)
{
  _displaycontrol |= LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_noBlink(void)
{
  _displaycontrol &= ~LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_blink(void)
{
  _displaycontrol |= LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_scrollDisplayLeft(void)
{
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd_scrollDisplayRight(void)
{
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcd_leftToRight(void)
{
  _displaymode |= LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | _displaymode);
}

void lcd_rightToLeft(void)
{
  _displaymode &= ~LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | _displaymode);
}

void lcd_autoScroll(void)
{
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | _displaymode);
}

void lcd_noAutoScroll(void)
{
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | _displaymode);
}

void lcd_createChar(UINT8 location, UINT8 charmap[])
{
  location &= 0x7; // We only have 8 locations 0-7
  lcd_command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    lcd_write(charmap[i]);
  }
}

void lcd_command(UINT8 value)
{
  lcd_send(value, LOW);
}

void lcd_write(UINT8 value)
{
  lcd_send(value, HIGH);
}

void lcd_send(UINT8 value, UINT8 mode)
{
  digitalWrite(_rs_pin, mode);

  if(_displayfunction & LCD_8BITMODE)
  {
    lcd_write8bits(value);
  }
  else
  {
    lcd_write4bits(value>>4);
    lcd_write4bits(value);
  }
}

void lcd_write4bits(UINT8 value)
{
  for(int i = 0; i < 4; i++)
  {
    pinMode(_data_pins[i], OUTPUT);
    digitalWrite(_data_pins[i], (value >> i) & 0x01);
  }

  lcd_pulseEnable();
}

void lcd_write8bits(UINT8 value)
{
  for(int i = 0; i < 8; i++)
  {
    pinMode(_data_pins[i], OUTPUT);
    digitalWrite(_data_pins[i], (value >> i) & 0x01);
  }

  lcd_pulseEnable();
}

void lcd_pulseEnable(void)
{
  digitalWrite(_enable_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(_enable_pin, HIGH);
  delayMicroseconds(1);    // enable pulse must be >450ns
  digitalWrite(_enable_pin, LOW);
  delayMicroseconds(100);   // commands need > 37us to settle
}

void lcd_printString(const char *s)
{
  while(*s) lcd_write(*s++);
}

void lcd_printNumber(long n, UINT8 base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *s = &buf[sizeof(buf) - 1];

  *s = '\0';

  // prevent crash if called with base == 1
  if(base < 2) base = 10;

  do {
    unsigned long m = n;
    n /= base;
    char c = m - base * n;
    *--s = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  while(*s) lcd_write(*s++);

  //  UINT8 i, j, di[10];
  //  for(i = 0; i < 10; i++) di[i] = 0;
  //  i = 0;
  //
  //  do {
  //    di[i++] = number % base;
  //    number = number / base;
  //  } while(number);
  //
  //  for(j = digit; j > 0; j--)
  //  {
  //    if(di[j - 1] < 10) Serial_write(di[j - 1] + '0');
  //    else Serial_write(di[j - 1] - 10 + 'A');
  //  }
}

void lcd_printFloat(double number, UINT8 digit)
{
  if(number > 4294967040.0) lcd_printString("ovf");  // constant determined empirically
  else if(number <-4294967040.0) lcd_printString ("ovf");  // constant determined empirically
  else
  {
    // Handle negative numbers
    if(number < 0.0)
    {
      lcd_write('-');
      number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (UINT8 i = 0; i < digit; ++i)
      rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    lcd_printNumber(int_part, DEC);

    // Print the decimal point, but only if there are digits beyond
    if(digit > 0) {
      lcd_write('.');
    }

    // Extract digits from the remainder one at a time
    while(digit-- > 0)
    {
      remainder *= 10.0;
      int toPrint = (int)remainder;
      lcd_printNumber(toPrint, DEC);
      remainder -= toPrint;
    }
  }
}
