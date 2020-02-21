#ifndef SURFACES_H
#define SURFACES_H

// Initializes the encoder's A and B pins.
void setup_encoder(void);

// Reads the encoders position based on A/B quadrature.
int read_encoder_position(void);
// Polls the encoder to see if the push button has been pressed.
// Returns whether or not the button has been pressed.
bool check_encoder_pushbutton(void);
#endif//SURFACES_H