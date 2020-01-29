
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "display.h"
#include "pins.h"

static LiquidCrystal lcdDisp(7, 8, 9, 10, 11, 12);

bool setup_display(void)
{
  // initialize the lcd to be 16x2 characters
  lcdDisp.begin(16, 2);
  lcdDisp.clear();

  // initialize pwm pins for rgb backlight
  pinMode(DISP_RGB_RED_CHANNEL, OUTPUT);
  pinMode(DISP_RGB_GREEN_CHANNEL, OUTPUT);
  pinMode(DISP_RGB_BLUE_CHANNEL, OUTPUT);


  return true;
}

void set_display_rgb(unsigned char r, 
                     unsigned char g, 
                     unsigned char b)
{
  analogWrite(DISP_RGB_RED_CHANNEL, r);
  analogWrite(DISP_RGB_GREEN_CHANNEL, g);
  analogWrite(DISP_RGB_BLUE_CHANNEL, b);
}

void display_write(const char *text, int x, int y)
{
  lcdDisp.setCursor(x, y);
  lcdDisp.print(text);
  lcdDisp.cursor();
}
