#include <Arduino.h>
#include "communication.h"

bool setup_comms(void)
{
  Serial.begin(9600);
  return true;
}
