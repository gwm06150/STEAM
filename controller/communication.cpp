#include <Arduino.h>
#include "communication.h"

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