#ifndef SURFACES_H
#define SURFACES_H

// Initializes the encoder's A and B pins.
void setup_encoder(void);

// Reads the encoders position based on A/B quadrature.
int read_encoder_position(void);

bool check_encoder_pushbutton(void);
#endif//SURFACES_H