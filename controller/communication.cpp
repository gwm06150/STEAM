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
  char num[4] = "";
  itoa(speed, num, 10);

  Serial.print("S");
  Serial.print(num);
  Serial.print("\n");
}

void send_speed_mode(unsigned char mode)
{
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
}