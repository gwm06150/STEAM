#ifndef PINS_H
#define PINS_H

// RGB LCD Display 
// rgb backlight
#define DISP_RGB_RED_CHANNEL   6 // PWM needed
#define DISP_RGB_GREEN_CHANNEL 5 // PWM needed
#define DISP_RGB_BLUE_CHANNEL  9 // PWM needed
// data 
#define DISP_RS                7
#define DISP_ENABLE            8
#define DISP_D4                13
#define DISP_D5                10
#define DISP_D6                11 
#define DISP_D7                12

// Encoder Quadrature and Switch 
#define ENC_A                  3 // interrupt needed
#define ENC_B                  2 // interrupt needed
#define ENC_PB                 A0 

// Auxiliary Buttons (Forward, Reverse, Stop)
#define AUX_1                  A1 // left, fwd 
#define AUX_2                  A2 // middle, stop 
#define AUX_3                  A3 // right, reverse

// Whistle/power switch on side of controller
#define MAIN_SWITCH            A4

#endif//PINS_H