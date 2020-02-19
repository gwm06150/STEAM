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

  delay(1000);
  display_clear();
}

float t = 0;
bool one = false;

void loop() 
{
  t += -.1;

  set_display_rgb(
    255*abs(cos(t/60.0)),
    255*abs(cos(t/60.0 + 1.4)),
    255*abs(cos(t/60.0 + 2.3))
    
  );

  {
    int c = read_encoder_position();
    char text[10] = "";

    itoa(c, text, 10);

    text[9] = '\0';
    
    display_write(text, 0, 0);
  }
}
