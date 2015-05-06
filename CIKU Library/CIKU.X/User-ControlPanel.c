#include "Arduino.h"
#include "LiquidCrystal.h"

int key;
char flag = 1;
unsigned long previousMillis = 0;
int interval = 1000;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(A0, INPUT);

  lcd_4bit(8, 9, 4, 5, 6, 7); // RS, E, D4, D5, D6, D7
  lcd_begin(16, 2);

  lcd_printString("   SHIELD-LCD   ");
  lcd_setCursor(0, 2);
  lcd_printString("Press any key...");

  while(analogRead(A0) > 1000);
  lcd_setCursor(0, 2);
  lcd_printString("Button:         ");
}

void loop()
{
  key = analogRead(A0);
  if(key < 1000 && flag == 1)
  {
    flag = 0;
    lcd_setCursor(7, 2);
    if(key < 60) lcd_printString("RIGHT    ");
    else if(key < 220) lcd_printString("UP       ");
    else if(key < 390) lcd_printString("DOWN     ");
    else if(key < 600) lcd_printString("LEFT     ");
    else if(key < 870) lcd_printString("SELECT   ");
  }
  else if(key > 1000 && flag == 0)
  {
    flag = 1;
    lcd_setCursor(0, 2);
    lcd_printString("Button:         ");
  }
  
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
    digitalToggle(LED);
  }
}
