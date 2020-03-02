
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "display.h"
#include "pins.h"

static unsigned char currentRed   = 0;
static unsigned char currentBlue  = 0;
static unsigned char currentGreen = 0;

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

void display_clear(void) 
{
  lcdDisp.clear();
}

void set_display_rgb(unsigned char r, 
                     unsigned char g, 
                     unsigned char b)
{
  // write rgb bytes directly to pwm pins
  analogWrite(DISP_RGB_RED_CHANNEL, r);
  analogWrite(DISP_RGB_GREEN_CHANNEL, g);
  analogWrite(DISP_RGB_BLUE_CHANNEL, b);

  currentRed = r;
  currentGreen = g;
  currentBlue = b;
}


static unsigned char approach(int cur, int targ, int step)
{
  if(targ > cur) {
    cur += step;
    if(cur >= targ)
      cur = targ; 
  }
  if(targ < cur) {
    cur -= step; 
    if(cur <= targ) 
      cur = targ;
  }

  return cur;
}

void smooth_display_rgb(unsigned char r,
                        unsigned char g,
                        unsigned char b,
                        unsigned int step)
{
  unsigned char nr = currentRed, 
                ng = currentGreen, 
                nb = currentBlue; 

  nr = approach(nr, r, step);
  ng = approach(ng, g, step);
  nb = approach(nb, b, step);

  set_display_rgb(nr,ng,nb);
}

void display_write(const char *text, int x, int y)
{
  // move the cursor in the display to the 
  // requested text location
  lcdDisp.setCursor(x, y);

  // print the text to the display 
  lcdDisp.print(text);
}
