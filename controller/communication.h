#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// Sets up the serial port for communication.
// returns: truth value indicating success
bool setup_comms(void);

// Sends a speed-change command over serial. 
void send_speed_cmd(unsigned char speed);

// Sends the appropriate command for stopping,
// reversing, or forward operation.
void send_speed_mode(unsigned char mode);

#endif//COMMUNICATION_H
