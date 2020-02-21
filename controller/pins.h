#ifndef PINS_H
#define PINS_H

// Display Related Pins 
#define DISP_RGB_RED_CHANNEL   9 // PWM needed
#define DISP_RGB_GREEN_CHANNEL 5 // PWM needed
#define DISP_RGB_BLUE_CHANNEL  6 // PWM needed
#define DISP_RS                7
#define DISP_ENABLE            8
#define DISP_D4                13
#define DISP_D5                10
#define DISP_D6                11 
#define DISP_D7                12

#define DISP_COLUMNS           16
#define DISP_ROWS              2


#define ENC_A                  3 // interrupt needed
#define ENC_B                  2 // interrupt needed
#define ENC_PB                 A0 
#endif//PINS_H