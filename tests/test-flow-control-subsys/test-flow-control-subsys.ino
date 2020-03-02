// Gene and John
// February 12, 2020
// Test Code for the flow controller subsystem

// Included Libraries
#include stdlib.h

// Define Pins
#define PIN_DIR 4
#define PIN_STEP 7
#define SOL_1 8 // first solenoid
#define SOL_2 9 // second solenoid

// Define State Names
#define SELF_TEST 0
#define LISTEN 1
#define ERROR 2

// Global variables
int engineState = SELF_TEST;
int period = 500;
int valveOnTimeStart1 = 0;
int minimumStepTimeFC = 5; // time in milliseconds

unsigned long time_now = 0; // this should take several days before running over



void setup() {
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(SOL_1, OUTPUT);
  pinMode(SOL_2, OUTPUT);
}



void loop() {
  // Read input from serial and set valves as needed

  // START OF SELF TEST STATE__________________________________________________________________________________________________________
if (engineState == SELF_TEST){
  // Set UP and Test Engine
  // If self test fails move to error State

  // If self test passes move to error state

  // FOR DEMONSTRATION
  if (serial.Read('1')){
    engineState = LISTEN;
    // if user prompts self test pass, move to listen
  }
  if (serial.Read('2')){
    engineState = ERROR;
    // if user prompts error, move to error state
  }
} // END OF SELF TEST STATE

// START OF ERROR STATE__________________________________________________________________________________________________________
if (engineState == ERROR){
  // Wait until the controller prompts to move back to self test
  // If controller prompts, move to self test

  // else, just wait

  // FOR DEMONSTRATION
  if (serial.Read('3')){
    engineState = SELF_TEST;
    // if the user provides all clear return to self test
  }
} // END OF ERROR STATE

//START OF LISTEN STATE__________________________________________________________________________________________________________
if (engineState == LISTEN){
  // Update Position of flow controller
  // get input angle, convert to steps, call steps until matched

  // Update cycle speed of valves

} // END OF LISTEN STATE
} // end of loop

void stepFlowValveClosed(){
  // used to step the flow contorller closed
  // millis used to control the width of the pulse
  // a min pulse width for the controller must be maintained in order for it to step properly

  // set the direction pin

  // raise the pusle pin high

  // log the start time of the pulse

  // once the pulse has gone on long enough lower the pulse pin and update the step count
}
void stepFlowValveOpen(){
  // used to step the flow contorller open
  // millis used to control the width of the pulse
  // a min pulse width for the controller must be maintained in order for it to step properly
  
  // set the direction pin

  // raise the pusle pin high

  // log the start time of the pulse

  // once the pulse has gone on long enough lower the pulse pin and update the step count

}
void solenoidValveTiming(){
  // based on the angle input from the encoder determine the angle of the valve
  
  // for sub system demo valve cycling will be based on time using millis() not angle
  // Set the first valve to open, once enough time has elapsed close first valve

  // Once the first valve is closed open the second valve, close when set time has elapsed

  // continue to loop this and update the delay between the valve swapping in the serial read area
}



  // for(int i = 0; i < 1600; i++) {
  //   digitalWrite(PIN_STEP, HIGH);
  //   delay(1);
  //   digitalWrite(PIN_STEP, LOW);
  //   delay(1);
  // }
  // delay(1000);
  
  // digitalWrite(PIN_DIR, LOW);
  
  // for(int i = 0; i < 1600; i++) {
  //   digitalWrite(PIN_STEP, HIGH);
  //   delay(1);
  //   digitalWrite(PIN_STEP, LOW);
  //   delay(1);
  // }
  // delay(1000);

