#include <Arduino.h>
#include "surfaces.h"
#include "pins.h"

#define ENCODER_WAITING 1
#define ENCODER_GO_CW   3
#define ENCODER_GO_CCW  5 

static volatile uint8_t encoderState = ENCODER_WAITING;
static volatile int encoderPosition = 0;

static void encoder_A_pulse(void);
static void encoder_B_pulse(void);

void setup_encoder(void)
{
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC_A), 
                  encoder_A_pulse,
                  FALLING);

  attachInterrupt(digitalPinToInterrupt(ENC_B),
                  encoder_B_pulse,
                  FALLING);

}

// Interrupt triggered by falling edge on encoder A
// channel.
static void encoder_A_pulse(void)
{
  switch(encoderState) {
  case ENCODER_WAITING:
    encoderState = ENCODER_GO_CW; 
    break; 
  case ENCODER_GO_CCW:
    encoderState = ENCODER_WAITING; 
    encoderPosition--;
    break;
  }
}

// Interrupt triggered by falling edge on encoder B 
// channel.
static void encoder_B_pulse(void)
{
  switch(encoderState) {
  case ENCODER_WAITING:
    encoderState = ENCODER_GO_CCW; 
    break; 
  case ENCODER_GO_CW:
    encoderState = ENCODER_WAITING; 
    encoderPosition++;
    break;
  }
}

int read_encoder_position(void)
{
  return encoderPosition; 
}