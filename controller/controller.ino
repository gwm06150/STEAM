#include "display.h"
#include "surfaces.h"
#include "communication.h"

void setup()
{
  setup_comms(); 

  setup_display();
  set_display_rgb(255,255,255);


}

void loop() 
{
  display_write("Hello", 0, 0);
  display_write("World", 0, 1);
}
