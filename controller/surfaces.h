#ifndef SURFACES_H
#define SURFACES_H


// Values for input bitmask (check_aux_buttons)
#define INPUT_AUX1         0b001 // auxiliary button 1 (left tilt switch)
#define INPUT_AUX2         0b010 // auxiliary button 2 (middle tilt switch)
#define INPUT_AUX3         4 // auxiliary button 3 (right tilt switch)


// Initializes the encoder's A and B pins.
void setup_encoder(void);

// Initializes the auxiliary buttons. 
void setup_buttons(void);

// Checks if the switch changed state to on 
// since last call.
bool check_switch_on(void);

// Return whether or not the switch is in the on position.
bool is_switch_on(void); 

// Reads the encoders position based on A/B quadrature.
// returns: absolute position of encoder 
int read_encoder_position(void);

// Fetches the last direction the knob was turned in.
// returns: true if cw
bool encoder_going_cw(void);

// Checks if the encoder recently changed position. 
// returns: true if a pulse occured
bool check_encoder_moved(void);

// Polls the encoder to see if the push button has been pressed.
// returns: truth value of whether or not the button is pressed 
bool check_encoder_pushbutton(void);

// Polls the auxiliary buttons to see if any have been pressed.
// returns: mask containing a combination of INPUT_ in menus.h
unsigned char check_aux_buttons(void);

#endif//SURFACES_H