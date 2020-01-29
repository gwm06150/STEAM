#include "display.h"
#include "surfaces.h"
#include "communication.h"

void setup()
{
  setup_comms(); 

  setup_display();
  set_display_rgb(0, 0, 0);
  display_write("LY9000 V1.0",0 ,0);
}

float t = 0;
bool one = false;

void loop() 
{
  t += 1.0;

  set_display_rgb(
    255*abs(cos(t/60.0)),
    255*abs(cos(t/60.0 + 1.4)),
    255*abs(cos(t/60.0 + 2.3))
    
  );
}
