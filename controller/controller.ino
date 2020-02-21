#include "display.h"
#include "surfaces.h"
#include "communication.h"


void setup()
{
  setup_comms(); 
  setup_encoder();
  setup_display();

  set_display_rgb(0, 0, 0);
  display_write("LY9000 V1.0",0 ,0);

  delay(5000);
  display_clear();
}

float t = 0;
int lastReading = 0;

unsigned long nextTick = 0;

bool testColor = false;

void loop() 
{
  
  unsigned long now = millis();

  if(now > nextTick) {
    nextTick = now + 100;

    lerp_display_rgb(testColor?255:0,testColor?0:255,0,4);
  }


  t += -.1;

  if(check_encoder_pushbutton()) {
    testColor = !testColor;
  }

  {
    int c = read_encoder_position();
    if(c != lastReading) {
      display_clear();
      lastReading = c;
    }

    char text[10] = "";

    itoa(c, text, 10);

    text[9] = '\0';
    
    display_write(text, 0, 0);
  }
}
