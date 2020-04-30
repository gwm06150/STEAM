#ifndef CONTROL_LOOPS_H
#define CONTROL_LOOPS_H

// Calculates the required step position of the flow controller 
// based on the PI control loop.
// 'dt' is the time in seconds since the last iteration of flow_control_loop, 
// 'targetRPM' is the desired RPM at output 
// 'currentRPM' is the current measured RPM at output.
int flow_control_loop(double dt, unsigned int targetRPM, unsigned int currentRPM);

#endif//CONTROL_LOOPS_Hc