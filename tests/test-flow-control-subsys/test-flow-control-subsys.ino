// Gene and John
// February 12, 2020
// Test Code for the flow controller subsystem

// Included Libraries
#include <string.h>


// Define Pins
// TX0, serial com
// TX1, serial com
#define PIN_DIR 20 // set pin high to close valve, flow controller servo direction
#define PIN_STEP 22 // pin to step flow controller servo
#define SOL_1 31 // first solenoid
#define SOL_2 33 // second solenoid
#define SOL_3 35 // third solenoid
#define SOL_4 37 // forth solenoid
#define SOL_5 39 // fifth solenoid
#define SOL_6 41 // sixth solenoid
#define SOL_7 43 // seventh solenoid
#define SOL_8 45 // eight solenoid
#define ENCODER_A 19 // encoder interupt 1, signal
#define ENCODER_B 20 // encoder interupt 2, signal
#define ENCODER_Z 21 // encoder interupt 3, index

// Define State Names
#define SELF_TEST 0
#define LISTEN 1
#define ERROR 2

// Define Values
#define PULSEWIDTH 1 // set in milliseconds
#define VALVESWITCHTIME 2000 // set in milliseconds
#define STOP 1
#define FORWARD 2
#define REVERSE 3

// Define Function Calls
#define READ_B bitRead(PIND, ENCODER_B) // faster than using the digital read function


// Global variables
unsigned long timeNow = 0; // this should take several days before running over
int engineState = LISTEN; // state the engine micro controller is operating in
unsigned long period = 500; // the period for the steper motor pulse
unsigned long pulseTimer = 0; // the timer to check the stepper motor pulse
int valveStepCount = 0; // the number of steps from home the valve stepper is
int pulseState = 0; // is the pulse high or low 
int solenoidState = 0; // is the solenoid triggered or not
unsigned long solenoidTimer = 0; // checks the length of time that the solenoid has been open
int valvePositionSet = 0; // the step count to which the flow controller valve has been set
int valvePositionActual = 0; // the position that the flow controller is at currently
int solenoidDirection = 1; // the direction in which the solenoids are being fired
char serialBuffer[50] = ""; // the buffer for serial communication with the pendant controller
unsigned int serialIndex = 0; // the index tracker for the serial buffer
 
volatile byte enc_state_A = LOW; // encoder channel A state
volatile byte enc_state_Z = LOW; // encoder channel Z state
int angle_count = 0; // the counter that keeps track of the angle of the engine


// Function Prototypes
// See functions for descriptions of functions
void stepFlowValveOpen();
void stepFlowValveClosed();
void enc_ch_a();
void enc_ch_z();

// Set up loop
void setup() {
  // Set up the serial coms at 9600 baud
  Serial.begin(9600);
  
  // Pins for flow controlller servo
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);

 // Pins for engine solenoids
  pinMode(SOL_1, OUTPUT);
  pinMode(SOL_2, OUTPUT);
  pinMode(SOL_3, OUTPUT);
  pinMode(SOL_4, OUTPUT);
  pinMode(SOL_5, OUTPUT);
  pinMode(SOL_6, OUTPUT);
  pinMode(SOL_7, OUTPUT);
  pinMode(SOL_8, OUTPUT);

  // Pins for encoder interupts
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), enc_ch_a, RISING);
  pinMode(ENCODER_B, INPUT);
  // attachInterrupt(digitalPinToInterrupt(ENCODER_B), enc_ch_b, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_Z), enc_ch_z, RISING);
} // END OF SET UP LOOP


void loop() { // start of main loop

  // Update Time at start of Loop
  timeNow = millis();


  while(Serial.available() > 0) { // *** START OF SERIAL SECTION ***
    // Read input from serial and set valves as needed
    char c = Serial.read();

    // if c is any character except a newline, store it 
    if(c != '\n') {
      serialBuffer[serialIndex] = c;
      serialIndex++;
    } else {
      // if c is a newline, run the command, then reset
      // run the command 
      switch(serialBuffer[0]) {
      case 's': solenoidDirection = STOP; // Stop engine
        break; 
      case 'f': solenoidDirection = FORWARD; // Forward engine
        break; 
      case 'r': solenoidDirection = REVERSE; // Rever engine
        break;
      case 'S': // preceeds the speed setting of the engine
        {
          int conversion = -1;
          conversion = atoi(serialBuffer+1); // get the speed value from the controller
          if(conversion != -1) {
            char buffer[10] = "";

            sprintf(buffer, "OK%d\n", conversion); // confirm the command. 

            valvePositionSet = 10 * conversion; // scales input value
            Serial.print(buffer);

          } else {
            Serial.print("NG\n"); // if the speed sent is no good send NG
          }

          Serial.flush(); // clear out and wait
        }
          break;

        default:
          break; 
      }

      // reset 
      memset(serialBuffer, 0, sizeof(char)*50);
      serialIndex = 0;
    }
  } // END OF SERIAL SECTION

  if (engineState == SELF_TEST){ // *** START OF SEFL TEST ***
    // Set UP and Test Engine
    // Check that the engine has supply pressure

    // Check that the flow controller is homed, if not home it

  } // END OF SELF TEST STATE


  if (engineState == ERROR){ // *** START OF ERROR ***
    // Wait until the controller prompts to move back to self test
    // If controller prompts, move to self test

    // else, just wait

  } // END OF ERROR STATE

  if (engineState == LISTEN){ // *** START OF LISTEN ***
    // change the solenoid valves as programmed
    if(solenoidDirection == 1){
      // Stop valve timing
      digitalWrite(SOL_1, LOW);
      digitalWrite(SOL_2, LOW);
    
    } else if(solenoidDirection == 2){
      // Start in forward condition
      solenoidValveTiming();
    
    } else if(solenoidDirection == 3){
      // Start in reverse condition
      solenoidValveTiming();
    }
    
    // call the appropriate step direction for the flow controller
    if(valvePositionSet > valveStepCount){
      // Open the valve to the set point
      stepFlowValveOpen();
    
    } else if(valvePositionSet < valveStepCount){
      // Close the valve to the set point
      stepFlowValveClosed();
    
    } else if(valvePositionSet == valveStepCount){
      // Do nothing
    }
  } // END OF LISTEN STATE
} // end of loop

void stepFlowValveClosed(){
  // McGuinness, John J.
  // used to step the flow contorller closed
  // millis() used to control the width of the pulse
  // a min pulse width for the controller must be maintained in order for it to step properly

  if(pulseState == 0){
    // set the direction pin
    digitalWrite(PIN_DIR, HIGH); // CLOSE VALVE
    
    // raise the pusle pin high
    digitalWrite(PIN_STEP, HIGH);
    
    // log the start time of the pulse
    pulseTimer = timeNow;

    // update the pulse state
    pulseState = 1;
  } else if (pulseState == 1){
    // check that the pulse has started
    if((timeNow - pulseTimer) >= PULSEWIDTH){
      // check that the pulse has gone on long enough
      
      // bring the pusle pin low
      digitalWrite(PIN_STEP, LOW);

      // Decrement the step count
      valveStepCount--;
      pulseState = 0;

    }
  }
} // END OF FLOW VALVE CLOSE

void stepFlowValveOpen(){
  // McGuinness, John J.
  // used to step the flow contorller closed
  // millis() used to control the width of the pulse
  // a min pulse width for the controller must be maintained in order for it to step properly
  if(pulseState == 0){
    // set the direction pin
    digitalWrite(PIN_DIR, LOW); // OPEN VALVE
    // raise the pusle pin high
    digitalWrite(PIN_STEP, HIGH);
    // log the start time of the pulse
    pulseTimer = timeNow;
    // update the pulse state
    pulseState = 1;

  } else if (pulseState == 1){
    // check that the pulse has started
    if((timeNow - pulseTimer) >= PULSEWIDTH){
      // check that the pulse has gone on long enough
      // bring the pusle pin low
      digitalWrite(PIN_STEP, LOW);
      // Decrement the step count
      valveStepCount++;
      pulseState = 0;
    }
  }
} // END OF FLOW VALVE OPEN

void solenoidValveTiming(){
  // McGuinness, John J.
  if(solenoidState == 0 && ((timeNow - solenoidTimer) >= VALVESWITCHTIME)){
    // start by firing solenoid 1
    digitalWrite(SOL_1, HIGH);
    digitalWrite(SOL_2, LOW);
    digitalWrite(SOL_3, LOW);
    digitalWrite(SOL_4, HIGH);
    
    // update the solenoid timer
    solenoidTimer = timeNow;

    // update the solenoid state
    solenoidState = 1;
  } else if(solenoidState == 1 && ((timeNow - solenoidTimer) >= VALVESWITCHTIME)){
    // start by firing solenoid 1
    digitalWrite(SOL_1, LOW);
    digitalWrite(SOL_2, HIGH);
    digitalWrite(SOL_3, HIGH);
    digitalWrite(SOL_4, LOW);

    // update the solenoid timer
    solenoidTimer = timeNow;

    // update the solenoid state
    solenoidState = 0;
  }
} // END OF SOLENOID VALVE TIMING

void enc_ch_a(){
  // McGuinnes, John J.
  // ISR to handle encoder channel A
  // If this function has been called then chanel A has encountered a rising edge. 
  noInterrupts();
  if(READ_B == HIGH){
    // if channel B is high the encoder is spining CW
    angle_count++;

  } else if (READ_B == LOW){
  // If channel B is low the encoder is spinning CCW
    angle_count--;
  } 
  interrupts();
} // END OF ENC_CH_A

void enc_ch_z(){
  // McGuinness, John J. 
  // ISR to handle encoder channel Z
  // Note that this channel only has to be used once immediately after power up once the engine has cycled one time.
  // the home position shouldn't really be drifting at all. The major sources of EMI are pretty far away in the 
  // control cluster and should not be able to mess with the encoder at all. At least as far as I know. 
  noInterrupts();
  angle_count = 0;
  // the angle keeps track of the number of pulses away from the home position that the encoder has turned.
  // if the encoder does not appear to be perfectly set onto the shaft of the engine for the timing, an offset can be added here
  interrupts(); 
} // END OF ENC_CH_Z
