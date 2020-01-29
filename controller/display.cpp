
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "display.h"
#include "pins.h"

static LiquidCrystal lcdDisp(
  DISP_RS,
  DISP_ENABLE,
  DISP_D4,
  DISP_D5,
  DISP_D6,
  DISP_D7
);

bool setup_display(void)
{
  // initialize the lcd to be 16x2 characters
  lcdDisp.begin(DISP_COLUMNS, DISP_ROWS);
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
  // write rgb bytes directly to pwm pins
  analogWrite(DISP_RGB_RED_CHANNEL, r);
  analogWrite(DISP_RGB_GREEN_CHANNEL, g);
  analogWrite(DISP_RGB_BLUE_CHANNEL, b);
}

void display_write(const char *text, int x, int y)
{
  // move the cursor in the display to the 
  // requested text location
  lcdDisp.setCursor(x, y);

  // print the text to the display 
  lcdDisp.print(text);
}
