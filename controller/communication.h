#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// Sets up the serial port for communication.
// returns: truth value indicating success
bool setup_comms(void);

// Sends a speed-change command over serial. 
void send_speed_cmd(unsigned char speed);

#endif//COMMUNICATION_H
