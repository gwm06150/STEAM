#include "display.h"
#include "surfaces.h"
#include "communication.h"
#include "defines.h"

#include <string.h>

#define MAX_FLOW_SET 500
#define MAX_ERROR_MSG 14

static unsigned int menuState = MENU_\UNINITIALIZED;
static unsigned int nextMenuState = MENU_UNINITIALIZED;

// error state variables 
static char errorMessage[MAX_ERROR_MSG] = "";

// speed control state variables 
static unsigned char speedMode = SPEED_STOPPED;
static short speedSetting = 0;

static char serialBuffer[50];
static unsigned int serialIndex = 0;

static bool expansionMode = false;

void setup()
{
  unsigned int bootTime;

  setup_comms(); 
  
  setup_encoder();
  setup_buttons();

  setup_display();

  set_display_rgb(40, 40, 40);
  display_write("LY9000 V1.0",0 ,0);

  // for 5 seconds, accept any combinations
  // that set flags
  bootTime = millis() + 1000;
  while(millis() < bootTime) {
    unsigned char buttons = check_aux_buttons();

    if(buttons == 7) {
      if(menuState == MENU_UNINITIALIZED) {
        display_write("ENTERING DEBUG", 0, 1);
        nextMenuState = MENU_DEBUG;
      }
    }

  }
  
  // if no special boot buttons were pressed, 
  // go into select mode by default
  if(nextMenuState == MENU_UNINITIALIZED) {
    nextMenuState = MENU_SELECT_MODE;
  }

  display_clear();
}

unsigned long nextTick = 0;
unsigned int testColor = 0;

void loop() 
{
  unsigned long now = millis();
  unsigned char buttons = 0;

  // manages serial 
  if(Serial.available() > 0){
    char c = Serial.read();

    if(c != '\n') {
      serialBuffer[serialIndex] = c;
      serialIndex++;
    } else {
      if(serialBuffer[0] == 'O' && serialBuffer[1] == 'K') {
        set_display_rgb(255,255,255);
        delay(50);
      } else if(serialBuffer[0] == 'N' && serialBuffer[1] == 'G') {
        set_display_rgb(255,255,0);
        delay(50);
      } else {
        switch(serialBuffer[0]) {

          case 'S': 
            {
              // we're receiving an RPM
              int conversion = -1;
              conversion = atoi(serialBuffer+1);

              // was the conversion successful?
              if(conversion != -1) {
                if(menuState == MENU_SETSPEED_VFC || menuState == MENU_SETSPEED_VVT){ 
                  char speed[4] = "";
                  
                  if(conversion < 999) 
                    itoa(conversion, speed, 10);
                  else {
                    // display error speed if its greater than 999..
                    // this shouldnt happen unless there was a glitch or 
                    // our engine is actually good
                    strcpy(speed, "XXX");
                  }

                  display_write("   ", 5, 0);
                  display_write(speed, 5, 0);
                  
                }
              }
            } 
            break;
          case 'E':
            {
              int i = 1;

              for(i = 1; serialBuffer[i] && serialBuffer[i] != '\n' && i < MAX_ERROR_MSG-1; i++){
                errorMessage[i-1] = serialBuffer[i];      
              }

              errorMessage[i+1] = '\0';

              // stop the engine just in case 
              send_speed_mode(SPEED_STOPPED);
              speedMode = SPEED_STOPPED;

              menuState = MENU_DEBUG;
              nextMenuState = MENU_ERROR;
            }
            break;
        }
      }

      serialIndex = 0;
      memset(serialBuffer, 0, sizeof(char)*50);
    }
  }

  // manages state changing and state entry
  if(nextMenuState != menuState) {
    menuState = nextMenuState;

    switch(menuState) {

    case MENU_ERROR: 
      display_clear();
      set_display_rgb(24,40,0);
      display_write(errorMessage, 0, 0);
      display_write("Knob to confirm", 0, 1);
      break;

    case MENU_SELECT_MODE:
      display_clear();
      display_write("  SELECT  MODE  ", 0, 0);
      display_write(" VVT        VFC ", 0, 1);
      set_display_rgb(24,24,24);
      break;
    case MENU_SETSPEED_VVT:
      display_clear();
      display_write("Cur:    RPM VVT", 0, 0);
      display_write("Set:    ", 0, 1);
      


      switch(speedMode) {
      case SPEED_FORWARD: 
        display_write("FWD", 12, 1);
        break;
      case SPEED_REVERSE:
        display_write("REV", 12, 1);
        break;
      case SPEED_STOPPED: 
        display_write("STP", 12, 1);
        break;
      }

      {
        char speed[4] = "";
        
        itoa(speedSetting, speed, 10);
        display_write("   ", 5, 1);
        display_write(speed, 5, 1);
      }

      break;
    case MENU_SETSPEED_VFC: 
      display_clear();
      display_write("Cur:    RPM VFC", 0, 0);
      display_write("Set:    ", 0, 1);

      switch(speedMode) {
      case SPEED_FORWARD: 
        display_write("FWD", 12, 1);
        break;
      case SPEED_REVERSE:
        display_write("REV", 12, 1);
        break;
      case SPEED_STOPPED: 
        display_write("STP", 12, 1);
        break;
      }

      {
        char speed[4] = "";
        
        itoa(speedSetting, speed, 10);
        display_write("   ", 5, 1);
        display_write(speed, 5, 1);
      }

      break;
    }
  }

  if(now > nextTick) {
    nextTick = now + 10;

    switch(menuState) {
      case MENU_DEBUG:
        switch(testColor) {
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
        break;

      case MENU_SETSPEED_VFC:
      case MENU_SETSPEED_VVT: 
        switch(speedMode) {
        case SPEED_FORWARD:
        case SPEED_REVERSE: 
          smooth_display_rgb(0,255,0,15);
          break;
        case SPEED_STOPPED: 
          smooth_display_rgb(255,0,0,15);
          break;
        }
        break;

      default:
        break;
    }
  }

  // check if the encoder has been pressed 
  if(check_encoder_pushbutton()) {
    switch(menuState){ 
    case MENU_ERROR: 
      nextMenuState = MENU_SELECT_MODE;
      send_ok();
      break; 
    case MENU_DEBUG:
      display_clear();
      display_write("Knob pressed.", 0, 0);

      testColor++;
      if(testColor == 3) testColor = 0;
      break;
    case MENU_SETSPEED_VFC: 
    case MENU_SETSPEED_VVT: 

      send_speed_cmd(speedSetting);

    default:
      break;
    }
  }

  // check if encoder has stepped 
  if(check_encoder_moved()) {
    switch(menuState) {
    case MENU_SETSPEED_VFC:
    case MENU_SETSPEED_VVT:
      if(encoder_going_cw()) {
        
        if(speedSetting+5 > MAX_FLOW_SET)
          speedSetting = MAX_FLOW_SET;
        else 
          speedSetting+=5;
      } else {
        if(speedSetting-5 < 0) 
          speedSetting = 0;
        else
          speedSetting-=5;
      }

      {
        char speed[4] = "";
        
        itoa(speedSetting, speed, 10);
        display_write("   ", 5, 1);
        display_write(speed, 5, 1);
      }

      break;
    }
  }

  // check if any of the auxiliary buttons have been pressed
  if((buttons = check_aux_buttons()) != 0) {
    
    switch(menuState) {
    case MENU_DEBUG:
      display_clear();
      display_write("Button Pressed", 0, 0);
      break;
    }
 
    if(buttons & INPUT_AUX1) {
      switch(menuState) {
      case MENU_DEBUG:
        display_write("1", 0, 1);
        break;

      case MENU_SELECT_MODE: 
        nextMenuState = MENU_SETSPEED_VVT;
        break;

      case MENU_SETSPEED_VFC: 
      case MENU_SETSPEED_VVT: 
        if(speedMode == SPEED_FORWARD)
          speedMode = SPEED_STOPPED;
        else 
          speedMode = SPEED_REVERSE;

          
        break;
      }
    }
    
    if(buttons & INPUT_AUX2) {
      switch(menuState) {
      case MENU_DEBUG:
        display_write("2", 1, 1);
        break;
      case MENU_SELECT_MODE: 
        // TODO: move to a proper location
        expansionMode = !expansionMode;
        send_expansion_mode(expansionMode);
        if(expansionMode)
          display_write("E", 6, 1);
        else 
          display_write(" ", 6, 1);
          
        break;

      case MENU_SETSPEED_VFC:
      case MENU_SETSPEED_VVT:
        if(speedMode == SPEED_STOPPED)
          nextMenuState = MENU_SELECT_MODE;
        else 
          speedMode = SPEED_STOPPED; 

        break;
      }
    }
    
    if(buttons & INPUT_AUX3) {
      switch(menuState) {
      case MENU_DEBUG:
        display_write("3", 2, 1);
        set_display_rgb(0,0,0);
        break;

      case MENU_SELECT_MODE: 
        nextMenuState = MENU_SETSPEED_VFC;
        break;
      
      case MENU_SETSPEED_VFC: 
      case MENU_SETSPEED_VVT: 
        if(speedMode == SPEED_REVERSE)
          speedMode = SPEED_STOPPED;
        else 
          speedMode = SPEED_FORWARD;
        break;
      }
    }

    switch(menuState) {
      case MENU_SETSPEED_VFC: 
      case MENU_SETSPEED_VVT: 
        switch(speedMode) {
        case SPEED_FORWARD: 
          display_write("FWD", 12, 1);
          break;
        case SPEED_REVERSE:
          display_write("REV", 12, 1);
          break;
        case SPEED_STOPPED: 
          display_write("STP", 12, 1);
          break;
        }

        send_speed_mode(speedMode);
        
        break; 
    }
  }

  switch(menuState) {
  case MENU_DEBUG: {
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
    break;
  }

}

