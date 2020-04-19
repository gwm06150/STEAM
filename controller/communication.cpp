#include <Arduino.h>
#include "communication.h"
#include "defines.h"

bool setup_comms(void)
{
  Serial.begin(9600);
  return true;
}

void send_speed_cmd(unsigned char speed)
{
  // create string containing speed number
  char num[4] = "";
  itoa(speed, num, 10);

  // send command in the format "S<speed>\n"
  Serial.print("S");
  Serial.print(num);
  Serial.print("\n");

  Serial.flush();
}

void send_speed_mode(unsigned char mode)
{
  // depending on the speed mode, send a lowercase f, r, or s.
  switch(mode) {
  case SPEED_FORWARD: 
    Serial.print("f\n");
    break;
  case SPEED_REVERSE:
    Serial.print("r\n");
    break;
  case SPEED_STOPPED:
    Serial.print("s\n");
    break;
  }

  Serial.flush();
}

void send_expansion_mode(bool enabled)
{
  Serial.print(enabled?"E":"e");
  Serial.flush();
}