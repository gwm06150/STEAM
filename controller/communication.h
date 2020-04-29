#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// Sets up the serial port for communication.
// returns: truth value indicating success
bool setup_comms(void);

// Sends a confirmation signal to the engine.
// Used during test procedure.
void send_ok(void);

// Sends a speed-change command over serial. 
void send_speed_cmd(int speed);

// Sends the appropriate command for stopping,
// reversing, or forward operation.
void send_speed_mode(unsigned char mode);

// Sends the appropriate command for enabling or 
// disabling the expansion phase of the timing code.
void send_expansion_mode(bool enabled);

#endif//COMMUNICATION_H
