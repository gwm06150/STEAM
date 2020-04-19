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
  #define SOL_1 31 // first solenoid
  #define SOL_2 33 // second solenoid
  #define SOL_3 35 // third solenoid
  #define SOL_4 37 // forth solenoid
  #define SOL_5 39 // fifth solenoid
  #define SOL_6 41 // sixth solenoid
  #define SOL_7 43 // seventh solenoid
  #define SOL_8 45 // eight solenoid
  
  // Engine encoder pins
  #define ENCODER_A 19 // encoder interupt 1, signal
  #define ENCODER_B 20 // encoder interupt 2, signal
  #define ENCODER_Z 21 // encoder interupt 3, index
  

// Define State Names
#define SELF_TEST 0
#define LISTEN 1
#define FAULT 2

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

volatile int angle_countA = 0; // the counter that keeps track of the angle of the engine A
volatile int angle_countB = 0; // the counter that keeps track of the angle of the engine B
volatile int loop_count = 0; // counts the number of whole loops

// int inAngle = 125;
// int woAngle = 1020;

// Function Prototypes
// See functions for descriptions of functions
void stepFlowValveOpen();
void stepFlowValveClosed();
void enc_ch_a();
void enc_ch_b();
void enc_ch_z();

// Set up loop
void setup() {
  // Set up the serial coms at 9600 baud
  Serial.begin(9600);
  
  // Pins for flow controlller servo
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);

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
  attachInterrupt(digitalPinToInterrupt(ENCODER_B), enc_ch_b, RISING);
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

    // Prompt the user to check that the flow controller is gently closed,
    // wait for the user to confirm the closure

    // Open the exhaust valves for all cylinders
    // Prompt the user to begin to the homing operation
    // "Clear engine area" 
    // Cycle the engine with fixed timing one time. 

  } // END OF SELF TEST STATE


  if (engineState == FAULT){ // *** START OF FAULT ***
    // Wait until the controller prompts to move back to self test
    // If controller prompts, move to self test

    // else, just wait

  } // END OF FAULT STATE

  if (engineState == LISTEN){ // *** START OF LISTEN ***
    // change the solenoid valves as programmed
    if(solenoidDirection == 1){
      // Stop valve timing
      EX_SOL_2_ON;
      EX_SOL_4_ON;
      EX_SOL_6_ON;
      EX_SOL_8_ON;
    
    } else if(solenoidDirection == 2){
      // Start in forward condition
      forward_valve_control();
    
    } else if(solenoidDirection == 3){
      // Start in reverse condition
      forward_valve_control();
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

void enc_ch_a(){
  // McGuinnes, John J.
  // ISR to handle encoder channel A
  // If this function has been called then channel A has encountered a rising edge. 
  angle_countA++;
} // END OF ENC_CH_A

void enc_ch_b(){
  // McGuinness, John J.
  // ISR to handle enocoder channel B
  // If this function has been called then channel B has encountered a rising edge.
  angle_countB++;
}

void enc_ch_z(){
  static int b = 0;
  
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

#define P1_ADMIT_FWD \
IN_SOL_1_ON;  \
EX_SOL_2_OFF; \
IN_SOL_3_OFF; \
EX_SOL_4_ON

#define P1_EXPAND_FWD \
IN_SOL_1_OFF;\
EX_SOL_2_OFF;\
IN_SOL_3_OFF; \
EX_SOL_4_ON

#define P1_EXHAUST \
IN_SOL_1_OFF;\
EX_SOL_2_ON;\
IN_SOL_3_OFF; \
EX_SOL_4_ON

#define P1_ADMIT_REV \
IN_SOL_1_OFF;\
EX_SOL_2_ON;\
IN_SOL_3_ON; \
EX_SOL_4_OFF

#define P1_EXPAND_REV \
IN_SOL_1_OFF;\
EX_SOL_2_ON;\
IN_SOL_3_OFF; \
EX_SOL_4_OFF



#define P2_ADMIT_FWD \
IN_SOL_5_ON;  \
EX_SOL_6_OFF; \
IN_SOL_7_OFF; \
EX_SOL_8_ON

#define P2_EXPAND_FWD \
IN_SOL_5_OFF;\
EX_SOL_6_OFF;\
IN_SOL_7_OFF; \
EX_SOL_8_ON

#define P2_EXHAUST \
IN_SOL_5_OFF;\
EX_SOL_6_ON;\
IN_SOL_7_OFF; \
EX_SOL_8_ON

#define P2_ADMIT_REV \
IN_SOL_5_OFF;\
EX_SOL_6_ON;\
IN_SOL_7_ON; \
EX_SOL_8_OFF

#define P2_EXPAND_REV \
IN_SOL_5_OFF;\
EX_SOL_6_ON;\
IN_SOL_7_OFF; \
EX_SOL_8_OFF


int deadZone = 100;

void forward_valve_control() 
{
  int phasedCount = angle_countA - 512; // 90 degrees lead
  if(phasedCount < 0) phasedCount += 2048; // wrap value if over 2048

  if(angle_countA > 0 && angle_countA < deadZone) {
    P1_EXHAUST;
  } else if(angle_countA > deadZone && angle_countA < 1024-deadZone) {
    P1_ADMIT_FWD;
  } else if(angle_countA > 1024-deadZone && angle_countA < 1024+deadZone) {
    P1_EXHAUST; 
  } else if(angle_countA > 1024+deadZone && angle_countA < 2048-deadZone) {
    P1_ADMIT_REV;
  }

  if(phasedCount > 0 && phasedCount < deadZone) {
    P2_EXHAUST;
  } else if(phasedCount > deadZone && phasedCount < 1024-deadZone) {
    P2_ADMIT_FWD;
  } else if(phasedCount > 1024-deadZone && phasedCount < 1024+deadZone) {
    P2_EXHAUST; 
  } else if(phasedCount > 1024+deadZone && phasedCount < 2048-deadZone) {
    P2_ADMIT_REV;
  }
}

// void forward_valve_control(){
//   int phasedCount = angle_countA - 512; // 90 degrees lead
//   if(phasedCount < 0) phasedCount += 2048; // wrap value if over 2048
  
//   // top half
//   if(angle_countA > 300 && angle_countA < inAngle){
//     // admitting air
//     P1_ADMIT_FWD;

//   } else if(angle_countA > inAngle && angle_countA < woAngle){
//     // expanding/working air
//     P1_EXPAND_FWD;

//   } else if(angle_countA > woAngle && angle_countA < 1024){
//     // exhausting air
//     P1_EXHAUST_FWD;
  
//   } else if((angle_countA - 1024) < inAngle){ // TODO: adjust for piston diameter difference
//     // admitting air
//     P1_ADMIT_REV;

//   } else if((angle_countA - 1024) > inAngle && (angle_countA - 1024) < woAngle){
//     // expanding/working air
//     P1_EXPAND_REV;

//   } else if((angle_countA - 1024) > woAngle){
//     // exhausting air
//     P1_EXHAUST_REV;
//   }

//   // top half
//   if(phasedCount > 300 && phasedCount < inAngle){
//     // admitting air
//     P2_ADMIT_REV;

//   } else if(phasedCount > inAngle && phasedCount < woAngle){
//     // expanding/working air
//     P2_EXPAND_REV;

//   } else if(phasedCount > woAngle && phasedCount < 1024){
//     // exhausting air
//     P2_EXHAUST_REV;
  
//   } else if((phasedCount - 1024) < inAngle){ // TODO: adjust for piston diameter difference
//     // admitting air
//     P2_ADMIT_FWD;

//   } else if((phasedCount - 1024) > inAngle && (phasedCount - 1024) < woAngle){
//     // expanding/working air
//     P2_EXPAND_FWD;

//   } else if((phasedCount - 1024) > woAngle){
//     // exhausting air
//     P2_EXHAUST_FWD;
//   }

// }

void reverse_valve_control(){

}

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
