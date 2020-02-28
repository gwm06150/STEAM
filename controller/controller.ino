#include "display.h"
#include "surfaces.h"
#include "communication.h"
#include "menus.h"

static unsigned int menuState = MENU_UNINITIALIZED;

bool flagDebugCode = false;

void setup()
{
  unsigned int bootTime;

  setup_comms(); 
  
  setup_encoder();
  setup_buttons();

  setup_display();

  set_display_rgb(0, 0, 0);
  display_write("LY9000 V1.0",0 ,0);

  // for 5 seconds, accept any combinations
  // that set flags
  bootTime = millis() + 5000;
  while(millis() < bootTime) {
    unsigned char buttons = check_aux_buttons();

    if(buttons == 7) {
      if(flagDebugCode == false) {
        display_write("ENTERING DEBUG", 0, 1);
      }
      flagDebugCode = true;
      
    }

  }
  
  display_clear();
}

unsigned long nextTick = 0;
unsigned int testColor = 0;

void loop() 
{
  unsigned long now = millis();
  unsigned char buttons = 0;

  if(now > nextTick) {
    nextTick = now + 10;

    if(flagDebugCode) switch(testColor) {
      case 0:
        set_display_rgb(255*abs(cos(now/2000.0)), 0, 0);
        break;
      case 1:
        set_display_rgb(0, 255*abs(cos(now/2000.0)), 0);
        break;
      case 2:
        set_display_rgb(0, 0, 255*abs(cos(now/2000.0)));
        break;
    }
  }

  // check if the encoder has been pressed 
  if(check_encoder_pushbutton()) {
    if(flagDebugCode) {
      display_clear();
      display_write("Knob pressed.", 0, 0);

      testColor++;
      if(testColor == 3) testColor = 0;
    }
  }

  // check if any of the auxiliary buttons have been pressed
  if((buttons = check_aux_buttons()) != 0) {
    display_clear();
    
    if(flagDebugCode) {
      display_write("Button Pressed", 0, 0);

      if(buttons & INPUT_AUX1) {
        display_write("1", 0, 1);
      }

      if(buttons & INPUT_AUX2) {

        display_write("2", 2, 1);
      }

      if(buttons & INPUT_AUX3) {
        display_write("3", 4, 1);
      }
    }
  }

  if(flagDebugCode){
    char num[5] = "";
    int  value = read_encoder_position();
    int i = 0;

    itoa(value, num, 10);

    for(i = 0; i < 4; i++) {
      if(!isdigit(num[i]) && num[i] != '-'){
        num[i] = ' ';
      }
    }

    num[4] = 0;

    display_write(num, 8, 1);
  }
}
