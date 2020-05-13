// Gene and John
// February 12, 2020
// Test Code for the flow controller subsystem

// Included Libraries
#include <string.h>

// Define Pins
  // TX0, serial com
  // TX1, serial com
  // Flow Controller
  #define PIN_DIR 47 // set pin high to close valve, flow controller servo direction
  #define PIN_STEP 49 // pin to step flow controller servo
  #define MOTOR_ENABLE 51 // pin to controll enable/disable for stepper in flow controller

  // Solenoids
  #define SOL_8 31 
  #define SOL_7 33 
  #define SOL_6 35 
  #define SOL_5 37 
  #define SOL_4 39 
  #define SOL_3 41 
  #define SOL_2 43 
  #define SOL_1 45 
  
  // Engine encoder pins
  #define ENCODER_A 19 // encoder interupt 1, signal
  #define ENCODER_B 20 // encoder interupt 2, signal
  #define ENCODER_Z 21 // encoder interupt 3, index
  

// Define State Names
#define ERROR 0
#define LISTEN 1
#define SELF_TEST 2

// Define Values
#define PULSEWIDTH 1 // set in milliseconds
#define VALVESWITCHTIME 2000 // set in milliseconds
#define STOP 1
#define FORWARD 2
#define REVERSE 3

// Define Function Calls
  // VALVES ON
  #define IN_SOL_1_ON digitalWrite(SOL_1, HIGH); // first solenoid open
  #define EX_SOL_2_ON digitalWrite(SOL_2, HIGH); // second solenoid open
  #define IN_SOL_3_ON digitalWrite(SOL_3, HIGH); // third solenoid open
  #define EX_SOL_4_ON digitalWrite(SOL_4, HIGH); // forth solenoid open
  #define IN_SOL_5_ON digitalWrite(SOL_5, HIGH); // fifth solenoid open
  #define EX_SOL_6_ON digitalWrite(SOL_6, HIGH); // sixth solenoid open
  #define IN_SOL_7_ON digitalWrite(SOL_7, HIGH); // seventh solenoid open
  #define EX_SOL_8_ON digitalWrite(SOL_8, HIGH); // eigth solenoid open

  // VALVES CLOSE
  #define IN_SOL_1_OFF digitalWrite(SOL_1, LOW); // first solenoid close
  #define EX_SOL_2_OFF digitalWrite(SOL_2, LOW); // second solenoid close
  #define IN_SOL_3_OFF digitalWrite(SOL_3, LOW); // third solenoid close
  #define EX_SOL_4_OFF digitalWrite(SOL_4, LOW); // forth solenoid close
  #define IN_SOL_5_OFF digitalWrite(SOL_5, LOW); // fifth solenoid close
  #define EX_SOL_6_OFF digitalWrite(SOL_6, LOW); // sixth solenoid close
  #define IN_SOL_7_OFF digitalWrite(SOL_7, LOW); // seventh solenoid close
  #define EX_SOL_8_OFF digitalWrite(SOL_8, LOW); // eigth solenoid close

  // Solenoid Macros
  #define P1_ADMIT_FWD {\
  IN_SOL_1_ON;  \
  EX_SOL_2_OFF; \
  IN_SOL_3_OFF; \
  EX_SOL_4_ON; }

  #define P1_EXPAND_FWD { \
  IN_SOL_1_OFF;\
  EX_SOL_2_OFF;\
  IN_SOL_3_OFF; \
  EX_SOL_4_ON; }

  #define P1_EXHAUST { \
  IN_SOL_1_OFF;\
  EX_SOL_2_ON;\
  IN_SOL_3_OFF; \
  EX_SOL_4_ON; }

  #define P1_ADMIT_REV { \
  IN_SOL_1_OFF;\
  EX_SOL_2_ON;\
  IN_SOL_3_ON; \
  EX_SOL_4_OFF; }

  #define P1_EXPAND_REV { \
  IN_SOL_1_OFF;\
  EX_SOL_2_ON;\
  IN_SOL_3_OFF; \
  EX_SOL_4_OFF; }

  #define P2_ADMIT_FWD { \
  IN_SOL_5_ON;  \
  EX_SOL_6_OFF; \
  IN_SOL_7_OFF; \
  EX_SOL_8_ON; } 

  #define P2_EXPAND_FWD { \
  IN_SOL_5_OFF;\
  EX_SOL_6_OFF;\
  IN_SOL_7_OFF; \
  EX_SOL_8_ON; }

  #define P2_EXHAUST { \
  IN_SOL_5_OFF;\
  EX_SOL_6_ON;\
  IN_SOL_7_OFF; \
  EX_SOL_8_ON; }

  #define P2_ADMIT_REV { \
  IN_SOL_5_OFF;\
  EX_SOL_6_ON;\
  IN_SOL_7_ON; \
  EX_SOL_8_OFF; }

  #define P2_EXPAND_REV { \
  IN_SOL_5_OFF;\
  EX_SOL_6_ON;\
  IN_SOL_7_OFF; \
  EX_SOL_8_OFF; }

  // WHISTLE DEFINES
  #define WHISTLE_TOOT 500 // In miliseconds for a short note 
  #define WHISTLE_PULL 1500 // In miliseconds for a short note
  #define WHISTLE_PIN 42 // pin to fire the whistl on or off 
  #define WHISTLE_ON digitalWrite(WHISTLE_PIN, HIGH) // turn the whistle on
  #define WHISTLE_OFF digitalWrite(WHISTLE_PIN, LOW) // turn the whistle off

  // ENCODER BIT READER
 // #define READ_B bitRead(PORTD, ENCODER_B) // check the logical state of encoder channel B
 #define READ_B digitalRead(ENCODER_B) 

  // STEPPER STUFF
  #define DISABLE_FLOW_CONTROLLER digitalWrite(MOTOR_ENABLE, HIGH) // diable the stepper motor to allow manual adjustment
  #define ENABLE_FLOW_CONTROLLER digitalWrite(MOTOR_ENABLE, LOW) // enable the stepper motor to lock adjustment and operate

// Global variables
unsigned long timeNow = 0; // this should take several days before running over
int engineState = SELF_TEST; // state the engine micro controller is operating in
unsigned long period = 500; // the period for the steper motor pulse
unsigned long pulseTimer = 0; // the timer to check the stepper motor pulse
int valveStepCount = 0; // the number of steps from home the valve stepper is
int pulseState = 0; // is the pulse high or low 
int solenoidState = 0; // is the solenoid triggered or not
unsigned long solenoidTimer = 0; // checks the length of time that the solenoid has been open
int valvePositionSet = 0; // the step count to which the flow controller valve has been set
int valvePositionActual = 0; // the position that the flow controller is at currently
int solenoidDirection = STOP; // the direction in which the solenoids are being fired
int lastSolenoidDirection = STOP; // keeps track for use with the whistle flags
char serialBuffer[50] = ""; // the buffer for serial communication with the pendant controller
unsigned int serialIndex = 0; // the index tracker for the serial buffer
volatile int angle_countA = 0; // the counter that keeps track of the angle of the engine A
volatile int angle_countB = 0; // the counter that keeps track of the angle of the engine B
volatile int loop_count = 0; // counts the number of whole loops
int test_counter = 0;
int error_message_control = 0;
bool test_running = false;
// Angular/count padding on either side of 
// the topdead and bottomdead center positions. 
// (Exhaust region)
int deadZone = 100;
// How many steps before the deadzones should the cylinder
// epand for?
int expandZone = 300;
int currentExpand = 0; 
int admitime = 0;
// whistle stuff
int whistleForwardHandler = 0;
int whistleReverseHandler = 0;
int whistleStopHandler = 0;
int whistleEnabled = 0;
int whistleTimer = 0;


int targetRPM = 0;
unsigned long nextControlCalc = 0;

// How many samples do we use for finding the RPM?
#define SPEED_SAMPLE_COUNT 8
unsigned long speedSamples[SPEED_SAMPLE_COUNT]; 
int lastAngle = 0;
int measuredRPM = 0; 
// timing variable to only send RPM 
// periodically (since it is an expensive calculation)
unsigned long nextRpmReadout = 0;
unsigned long nextRpmCalc = 0;
// Send an RPM readout every second
#define TIME_BETWEEN_READOUTS 1000
#define TIME_BETWEEN_RPM_CALC 50

// Function Prototypes
// See functions for descriptions of functions
void stepFlowValveOpen();
void stepFlowValveClosed();
void enc_ch_a();
void enc_ch_b();
void enc_ch_z();
void calculate_rpm();

void valve_reverse_control(int angle, bool expansion);
void valve_forward_control(int angle, bool expansion);
void solenoidValveTiming();

void whistle_engine_forward();
void whistle_engine_reverse();
void whistle_engine_stop();

// Set up loop
void setup() {
  // Set up the serial coms at 9600 baud
  Serial.begin(9600);
  
  // Pins for flow controlller servo
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);

  // Pins for the whistle
  pinMode(WHISTLE_PIN, OUTPUT);

 // Pins for engine solenoids
  pinMode(SOL_1, OUTPUT);
  pinMode(SOL_2, OUTPUT);
  pinMode(SOL_3, OUTPUT);
  pinMode(SOL_4, OUTPUT);
  pinMode(SOL_5, OUTPUT);
  pinMode(SOL_6, OUTPUT);
  pinMode(SOL_7, OUTPUT);
  pinMode(SOL_8, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  // Pins for encoder interupts
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), enc_ch_a, RISING);
  // attachInterrupt(digitalPinToInterrupt(ENCODER_B), enc_ch_b, RISING);
      // as of the moment we think that we can get away with only the a channel for positioning the engine and
      // controlling the timing of the valve.
  attachInterrupt(digitalPinToInterrupt(ENCODER_Z), enc_ch_z, RISING);
  pinMode(ENCODER_B, INPUT); // used in checking the dirction of turn of the engine. 
} // END OF SET UP LOOP

// temporary variable so that the controller 
// can enable and disable experimental expansion 
// timing 
bool expansionEnabled = true; 

void loop() { // start of main loop
  unsigned long lastTime = timeNow;
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
      case 'P':
        engineState = ERROR;
        break; 
      case 'W':
        whistleEnabled = 1;
        break;
      case 'w':
        whistleEnabled = 0;
        break;
      case 'e': expansionEnabled = false; 
        Serial.print("OK\n");
        Serial.flush();
        break; 
      case 'E': expansionEnabled = true; 
        Serial.print("OK\n");
        Serial.flush();
        break; 
      case 'K':
        if(engineState == ERROR && error_message_control == 1){
          if(!test_running) {
            test_running = true;

            if(test_counter == 0) {
              // make the valve want to close by 500 steps
              valveStepCount = 5000;
              valvePositionSet = 0; 

              ENABLE_FLOW_CONTROLLER;
            }

            if(test_counter == 1){
              solenoidTimer = timeNow;
              valvePositionSet = 800;
            }
          }
        }
      case 's': solenoidDirection = STOP; // Stop engine
        currentExpand = 0;
        measuredRPM = 0;
        for(int i = 0; i < SPEED_SAMPLE_COUNT; i++)
          speedSamples[i] = 0;

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

            //valvePositionSet = 10 * conversion; // scales input value
            targetRPM = conversion;
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

  if (engineState == ERROR){ // *** START OF SELF TEST ***   

    // Message Sender Section:
    if(test_counter == 0 && error_message_control == 0){
      // send the message to close the flow controller
      Serial.print("E");
      Serial.print("Home FC:");
      Serial.print("\n");
      Serial.flush();
      error_message_control = 1;
      test_running = false;

    } else if(test_counter == 1 && error_message_control == 0){
      // set the message to spin the engine
      Serial.print("E");
      Serial.print("Home Engine:");
      Serial.print("\n");
      Serial.flush();
      error_message_control = 1;
      test_running = false;

    }

    if(test_running) {
  
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

      switch(test_counter) {
        case 0:
          
          // this occurs after stepping closed 500 steps 
          if(valveStepCount == 0) {
            error_message_control = 0;
            test_counter++;
          }
          break; 
        case 1:
          // run test 
          engine_homing_procedure();

          if(loop_count > 0) {
            error_message_control = 0;
            test_counter = 0;
            engineState = LISTEN; 
             
            // we're done with our tests, so send a finalizing 
            // K 
            Serial.print("K");
            Serial.print("\n");
            Serial.flush();
          }
          break; 
      }
    }

  } // END OF SELF TEST STATE

  if (engineState == LISTEN){ // *** START OF LISTEN ***
    
    // change the running direction of the engine
    if(solenoidDirection == STOP){
      // Stop valve timingS
      // do nothing
    } else if(solenoidDirection == FORWARD){
      // Start in forward condition
      valve_forward_control(angle_countA, expansionEnabled);
    } else if(solenoidDirection == REVERSE){
      // Start in reverse condition
      valve_reverse_control(angle_countA, expansionEnabled);
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

    // set the appropriate flag for the whistle
    if(whistleEnabled == 1){
      if((lastSolenoidDirection == FORWARD || lastSolenoidDirection == REVERSE) && solenoidDirection == STOP){
        // sound the stop signal 
        whistleStopHandler = 1;
        whistleReverseHandler = 0;
        whistleForwardHandler = 0;
        whistleTimer = timeNow;
      } 
      if(lastSolenoidDirection == STOP && solenoidDirection == FORWARD){
        // sound the forward signal
        whistleForwardHandler = 1;
        whistleReverseHandler = 0;
        whistleStopHandler = 0;
        whistleTimer = timeNow;
      } 
      if(lastSolenoidDirection == STOP && solenoidDirection == REVERSE){
        // sound the reverse signal
        whistleReverseHandler = 1;
        whistleForwardHandler = 0;
        whistleStopHandler = 0;
        whistleTimer = timeNow;
      }
      
    }

    // calling the whistle functions as needed.
    if(whistleForwardHandler == 1){
      // call the forward whistle function
      whistle_engine_forward();
    } 
    if(whistleReverseHandler == 1){ 
      // call the reverse whistle function
      whistle_engine_reverse();
    } 
    if(whistleStopHandler == 1){
      // call the stop whistle handler
      whistle_engine_stop();
    }

    lastSolenoidDirection = solenoidDirection;
      
      
    // calculate the RPM of engine and update
    if(timeNow > nextRpmCalc){
      // reset timer 
      nextRpmCalc = timeNow + TIME_BETWEEN_RPM_CALC;

      for(int i = 0; i < SPEED_SAMPLE_COUNT-1; i++) {
        speedSamples[i] = speedSamples[i+1];
      }
      
      speedSamples[SPEED_SAMPLE_COUNT-1] = angle_countA - lastAngle; 
      lastAngle = angle_countA;

      // recalculate RPM 
      calculate_rpm();
    }

    if(timeNow > nextRpmReadout) {
      char buffer[6] = "";

      nextRpmReadout = timeNow + TIME_BETWEEN_READOUTS;
      // put rpm into S code message 
      sprintf(buffer, "S%d\n", measuredRPM);
      // send readout to controller 
      Serial.print(buffer);
      Serial.flush();
    }

    if(timeNow > nextControlCalc) {
      //valvePositionSet = 10 * targetRPM;
      flow_control(timeNow - lastTime);
      nextControlCalc = timeNow + 50;
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


const int Kp = 50;
const int Ki = 1;
int accumulate = 0;
void flow_control(unsigned long dt) 
{
  int error = targetRPM - measuredRPM;

  //valvePositionSet = Kp*targetRPM + 10*error;
  if(error > 0)
    valvePositionSet+=10;
  else if(error < 0) 
    valvePositionSet-=10;

  if(valvePositionSet > 2000)
    valvePositionSet = 2000;
  else if(valvePositionSet < 0)
    valvePositionSet = 0;
}


unsigned long lastAtime = 0;
int special = 0;

void enc_ch_a(){
  
  // McGuinnes, John J.
  // ISR to handle encoder channel A
  // If this function has been called then channel A has encountered a rising edge. 
  if(!READ_B){
    angle_countA++; // if forward direction increment

    if(angle_countA > 2048)
      angle_countA -= 2048;
  } else{
    angle_countA--; // if reverse direction decrement
    
    if(angle_countA < 0)
      angle_countA += 2048;
  }
  // if the forward valve control loop appears to have broken then remove the ! operator from the if statement
  // this should swap the increment and decrement case, fixing things. 
  // the addtion of decrement was to acount for any potential backwards drift during the homing process. though
  // it might prove useful if we see something weird during the engine running.  
} // END OF ENC_CH_A

void enc_ch_b(){
  // McGuinness, John J.
  // ISR to handle enocoder channel B
  // If this function has been called then channel B has encountered a rising edge.
  angle_countB++;
}

void enc_ch_z(){
  static int b = 0;

  

  if(currentExpand < expandZone)
    currentExpand += 100;

   // McGuinness, John J. 
   // ISR to handle encoder channel Z
   // Note that this channel only has to be used once immediately after power up once the engine has cycled one time.
   // the home position shouldn't really be drifting at all. The major sources of EMI are pretty far away in the 
   // control cluster and should not be able to mess with the encoder at all. At least as far as I know. 
  //noInterrupts();
  loop_count++;
  angle_countA = 0;
  angle_countB = 0;
   //the angle keeps track of the number of pulses away from the home position that the encoder has turned.
   // if the encoder does not appear to be perfectly set onto the shaft of the engine for the timing, an offset can be added here
  //interrupts(); 

  pinMode(LED_BUILTIN, (b = !b, b? HIGH: LOW));

} // END OF ENC_CH_Z

void valve_forward_control(int angle, bool expansionMode) {
  // Valve Timing Function 
  // 'angle': the current angle in steps, 0-2048
  // 'expansionMode': should we try to expand to maximize efficiency? (temp, experimental)
  int phasedAngle = angle - 512; // 90 degrees lead
  if(phasedAngle < 0) phasedAngle += 2048; // wrap value if over 2048

  // Piston 1 Timings
  if(angle > 0 && angle < deadZone) {
    // Deadzone before admitting 
    P1_EXHAUST;
  } else if(angle > deadZone && angle < 1024-deadZone-currentExpand) {
    P1_ADMIT_FWD;
  } else if(angle > 1024-deadZone-currentExpand && angle < 1024-deadZone) {
    if(expansionMode) {
      P1_EXPAND_FWD;
    } else {
      P1_ADMIT_FWD;
    }

  } else if(angle > 1024-deadZone && angle < 1024+deadZone) {
    // Bottom deadzone 
    P1_EXHAUST; 
  } else if(angle > 1024+deadZone && angle < 2048-deadZone-currentExpand) {
    // Second admission region, needs to be the opposite of the first.
    P1_ADMIT_REV; 
  } else if(angle > 2048-deadZone-currentExpand && angle < 2048-deadZone) {
    // Second expansion region, do we try? 
    // Make sure direction matches previous phase.
    if(expansionMode) {
      P1_EXPAND_REV;
    } else {
      P1_ADMIT_REV;
    }
  } else if(angle > 2048-deadZone && angle < 2048) {
    P1_EXHAUST;
  }

  // Piston 2 Timings
  if(phasedAngle > 0 && phasedAngle < deadZone) {
    // Deadzone before admitting 
    P2_EXHAUST;
  } else if(phasedAngle > deadZone && phasedAngle < 1024-deadZone-currentExpand) {
    // First admission region 
    P2_ADMIT_FWD;
  } else if(phasedAngle > 1024-deadZone-currentExpand && phasedAngle < 1024-deadZone) {
    // First expansion region, do we try?
    if(expansionMode) {
      P2_EXPAND_FWD;
    } else {
      P2_ADMIT_FWD;
    }
  } else if(phasedAngle > 1024-deadZone && phasedAngle < 1024+deadZone) {
    // Bottom deadzone 
    P2_EXHAUST; 
  } else if(phasedAngle > 1024+deadZone && phasedAngle < 2048-deadZone-currentExpand) {
    // Second admission region, needs to be the opposite of the first.
    P2_ADMIT_REV; 
  } else if(phasedAngle > 2048-deadZone-currentExpand && phasedAngle < 2048-deadZone) {
    // Second expansion region, do we try? 
    // Make sure direction matches previous phase.
    if(expansionMode) {
      P2_EXPAND_REV;
    } else {
      P2_ADMIT_REV;
    }
  } else if(phasedAngle > 2048-deadZone && phasedAngle < 2048) {
    P2_EXHAUST;
  }
}

void valve_reverse_control(int angle, bool expansionMode){ 
  // calulate the admission time (This makes thing easier for John)
  admitime = 1024 - (deadZone*2) - currentExpand;

  // Angle Phasing and Wrapping for Piston 2
  int phasedAngle = angle - 512; // 90 degrees lead
  if(phasedAngle < 0) phasedAngle += 2048; // wrap value if over 2048
  //if(phasedAngle > 2048) phasedAngle -= 2048;

  // Piston 1
  if(2048 >= angle && angle > (2048-deadZone)){
    P1_EXHAUST;
  } else if((2048-deadZone) >= angle && angle > (2048-deadZone-admitime)){
    P1_ADMIT_FWD;
  } else if((2048-deadZone-admitime) >= angle && angle > (1024+deadZone)){
    if(expansionMode){
      P1_EXPAND_FWD;
    } else{
      P1_ADMIT_FWD
    }
  } else if((1024+deadZone) >= angle && angle > (1024-deadZone)){
    P1_EXHAUST;
  } else if((1024-deadZone) >= angle && angle > (1024-deadZone-admitime)){
    P1_ADMIT_REV;
  } else if((1024-deadZone-admitime) >= angle && angle > deadZone){
    if(expansionMode){
      P1_EXPAND_REV;
    } else{
      P1_ADMIT_REV;
    }
  } else if (deadZone >= angle && angle > 0){
    P1_EXHAUST;
  }

  // Piston 2
  if(2048 >= phasedAngle && phasedAngle > (2048-deadZone)){
    P2_EXHAUST;
  } else if((2048-deadZone) >= phasedAngle && phasedAngle > (2048-deadZone-admitime)){
    P2_ADMIT_FWD;
  } else if((2048-deadZone-admitime) >= phasedAngle && phasedAngle > (1024+deadZone)){
    if(expansionMode){
      P2_EXPAND_FWD;
    } else{
      P2_ADMIT_FWD
    }
  } else if((1024+deadZone) >= phasedAngle && phasedAngle > (1024-deadZone)){
    P2_EXHAUST;
  } else if((1024-deadZone) >= phasedAngle && phasedAngle > (1024-deadZone-admitime)){
    P2_ADMIT_REV;
  } else if((1024-deadZone-admitime) >= phasedAngle && phasedAngle > deadZone){
    if(expansionMode){
      P2_EXPAND_REV;
    } else{
      P2_ADMIT_REV;
    }
  } else if (deadZone >= phasedAngle && phasedAngle > 0){
    P2_EXHAUST;
  }
}

void calculate_rpm(void){
  // take the average of the speed samples 
  unsigned long sum = 0;
  for(int i = 0; i < SPEED_SAMPLE_COUNT; i++) 
    sum += speedSamples[i];
  
  sum /= SPEED_SAMPLE_COUNT; 

  if(sum != 0) {
    // avg delta steps * (1 rev / 2048 steps) 
    //                 / dt 
    // ms * (1 sec / 1000 ms) * (1 min / 60 sec)
    //double rpm = 1.0/((((double)sum)/1000.0)*(1.0/60.0));
    

    double rpm = ((double)sum / 2048)/(TIME_BETWEEN_RPM_CALC/60000.0);

    // round and store the RPM
    measuredRPM = (int)round(rpm);
  } else {
    measuredRPM = 0;
  }
}

void whistle_engine_forward(){
  int delta = 0;
  delta = timeNow - whistleTimer;
  if(delta < 1000){
    // on for 1 second
    WHISTLE_ON;
  } else if(delta >= 1000 && delta < 1300){
    // off for 0.3 seconds
    WHISTLE_OFF;
  } else if(delta >= 1300 && delta < 2300){
    // on for 1 second
    WHISTLE_ON;
  } else{
    WHISTLE_OFF;
    whistleForwardHandler = 0;
  }
  
}

void whistle_engine_stop(){
  int delta = 0;
  delta = timeNow - whistleTimer;
  if(delta < 1500){
    // on for 1.5 seconds
    WHISTLE_ON;
  } else{
    // off, and reset the handler
    WHISTLE_OFF;
    whistleStopHandler = 0;
  }

}

void whistle_engine_reverse(){
  int delta = 0;
  delta = timeNow - whistleTimer;
  if(delta < 300){
    // on for 0.3s
      WHISTLE_ON;
  } else if(delta >= 300 && delta < 500){
    // off for 0.2s
    WHISTLE_OFF;
  } else if(delta >= 500 && delta < 800){
    // on for 0.3s
    WHISTLE_ON;
  } else if(delta >= 800 && delta < 1000){
    // off for 0.2s
    WHISTLE_OFF;
  } else if(delta >= 1000 && delta < 1300){
    // on for 0.3s
    WHISTLE_ON;
  } else{
    WHISTLE_OFF;
    whistleReverseHandler = 0;
  }
}

void engine_homing_procedure(){
  int time = timeNow - solenoidTimer; 

  if(time < 1000) {
    P1_ADMIT_FWD;
    P2_EXHAUST;
  } else if(time < 2000) {
    P2_ADMIT_FWD;
    P1_EXHAUST;
  } else if(time < 3000) {
    P1_ADMIT_REV;
    P2_EXHAUST;
  } else if(time < 4000) {
    P2_ADMIT_REV;
    P1_EXHAUST;
  } else if(time < 5000) {
    solenoidTimer = timeNow;
  }
} // END OF SOLENOID VALVE TIMING