#include <Arduino.h>
#include "surfaces.h"
#include "pins.h"

#define PULSE_TIMEOUT 80


// Encoder related state 
static volatile int           encoderPosition = 0;
static volatile unsigned long nextPulse       = 0;
static          bool          lastPB          = false;
static          bool          wasGoingCW      = false;
static          bool          pulseHappened   = false;
// Auxiliary button related state 
static          unsigned char lastButtons     = 0;

// Interrupt handler for B channel of encoder 
static void encoder_B_pulse(void);



void setup_encoder(void)
{
  // set up quadrature and pushbutton 
  // as inputs. the encoder breakout board
  // has built in pullups.
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  pinMode(ENC_PB, INPUT);

  // attach interrupt handler to the B channel of the 
  // encoder 
  attachInterrupt(digitalPinToInterrupt(ENC_B),
                  encoder_B_pulse,
                  RISING);

}



void setup_buttons(void)
{
  // set up the three aux buttons as pullup inputs
  // wired as active low (connect to ground)
  pinMode(AUX_1, INPUT_PULLUP);
  pinMode(AUX_2, INPUT_PULLUP);
  pinMode(AUX_3, INPUT_PULLUP);
}




// Interrupt triggered by rising edge on encoder B 
// channel.
static void encoder_B_pulse(void)
{
  // get current time
  unsigned long now = millis();

  // only let the interrupt "trigger" if 
  // there has been sufficient time since
  // the last 
  if(now > nextPulse) {
    nextPulse = now + PULSE_TIMEOUT;

    pulseHappened = true;

    if(digitalRead(ENC_A) == HIGH) {
      
      encoderPosition--;
      wasGoingCW = false;
    } else {
      encoderPosition++;
      wasGoingCW = true;
    } 
  }
}



int read_encoder_position(void)
{
  return encoderPosition; 
}

bool check_encoder_pushbutton(void)
{
  bool curButtonState = digitalRead(ENC_PB);
  
  if(curButtonState != lastPB) {
    lastPB = curButtonState;
    return curButtonState == LOW; 
  }

  return false;
}


bool encoder_going_cw(void)
{
  return wasGoingCW;
}

bool check_encoder_moved(void)
{
  // consume the pulse and return true
  if(pulseHappened) {
    pulseHappened = false; 
    return true;
  }

  return false;
}



unsigned char check_aux_buttons(void)
{
  unsigned char newMask = 0;

  // build the button bitmask based on the current state of the 
  // aux pins. if they're low, that means the button is pressed
  newMask |= (digitalRead(AUX_1) == LOW)? INPUT_AUX1: 0;
  newMask |= (digitalRead(AUX_2) == LOW)? INPUT_AUX2: 0;
  newMask |= (digitalRead(AUX_3) == LOW)? INPUT_AUX3: 0;
  
  // if there has been a state change 
  if(newMask != lastButtons) {
    // update the current button state in memory
    lastButtons = newMask; 
    return newMask;
  }

  return 0;
}