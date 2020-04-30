#include "control_loops.h"

double integrator = 0;
double Kp = 1;
double Ki = 1;

int flow_control_loop(double dt,
                      unsigned int targetRPM, 
                      unsigned int currentRPM)
{
  int error = targetRPM - currentRPM;

  integrator += Ki*error*dt; 

  int output = integrator + Kp*error;

  return output;
}