#include <Arduino.h>
#include "surfaces.h"
#include "pins.h"

static volatile int           encoderPosition = 0;
static volatile unsigned long nextPulse       = 0;

static void encoder_B_pulse(void);

void setup_encoder(void)
{
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENC_B),
                  encoder_B_pulse,
                  RISING);

}



// Interrupt triggered by rising edge on encoder B 
// channel.
static void encoder_B_pulse(void)
{
  unsigned long now = millis();

  if(now > nextPulse) {
    nextPulse = now + 50;

    if(digitalRead(ENC_A) == HIGH) {
      encoderPosition++;
    } else {
      encoderPosition--;
    } 
  }
}

int read_encoder_position(void)
{
  return encoderPosition; 
}