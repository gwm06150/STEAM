// Gene and John
// February 12, 2020
// Test Code for the flow controller subsystem

// Included Libraries

// Define Pins
#define PIN_DIR 4 // set pin high to close valve
#define PIN_STEP 7
#define SOL_1 8 // first solenoid
#define SOL_2 9 // second solenoid

// Define State Names
#define SELF_TEST 0
#define LISTEN 1
#define ERROR 2

// Define Values
#define pulseWidth 1 // set in milliseconds
#define valveSwitchTime 2000 // set in milliseconds

// Global variables
unsigned long timeNow = 0; // this should take several days before running over
int engineState = LISTEN;
int period = 500;
int pulseTimer = 0;
int valveStepCount = 0;
int pulseState = 0;
int solenoidState = 0;
int solenoidTimer = 0;
int valvePositionSet = 0;
int valvePositionActual = 0;
int solenoidDirction = 0;

// Function Prototypes
void stepFlowValveOpen();
void stepFlowValveClosed();

void setup() {
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(SOL_1, OUTPUT);
  pinMode(SOL_2, OUTPUT);
}

void loop() {
  // Read input from serial and set valves as needed

  // S<number>\n 
  // r\n
  // f\n 
  // s\n 

  while(Serial.available() > 0) {
    char c = Serial.read();

    switch(c) {
    case 'r': solenoidDirction = 3; break; 
    case 'f': solenoidDirction = 2; break; 
    case 's': solenoidDirction = 1; break;
    default: break;
    }
  }

  // Update Time at start of Loop
  timeNow = millis();

  // START OF SELF TEST STATE__________________________________________________________________________________________________________
  if (engineState == SELF_TEST){
    // Set UP and Test Engine
    // If self test fails move to error State

    // If self test passes move to error state

  } // END OF SELF TEST STATE

  // START OF ERROR STATE__________________________________________________________________________________________________________
  if (engineState == ERROR){
    // Wait until the controller prompts to move back to self test
    // If controller prompts, move to self test

    // else, just wait

  } // END OF ERROR STATE

  //START OF LISTEN STATE__________________________________________________________________________________________________________
  if (engineState == LISTEN){
    
    // change the solenoid valves as programmed
    if(solenoidDirction == 1){
      // Stop valve timing
      digitalWrite(SOL_1, LOW);
      digitalWrite(SOL_2, LOW);

    }
    else if(solenoidDirction == 2){
      // Start in forward condition
      solenoidValveTiming();

    }
    else if(solenoidDirction == 3){
      // Start in reverse condition
      solenoidValveTiming();

    }
    
    
    
    solenoidValveTiming();

    // 
    if(valvePositionSet < valveStepCount){
      // Open the valve to the set point
      stepFlowValveOpen();
    }
    else if(valvePositionSet > valveStepCount){
      // Close the valve to the set point
      stepFlowValveClosed();
    }
    else if(valvePositionSet == valveStepCount){
      // Do nothing
    }

  } // END OF LISTEN STATE
} // end of loop

void stepFlowValveClosed(){
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
  }

  if(pulseState == 1){
    // check that the pulse has started
    if((timeNow - pulseTimer) >= pulseWidth){
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
  }

  if(pulseState == 1){
    // check that the pulse has started
    if((timeNow - pulseTimer) >= pulseWidth){
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
  if(solenoidState == 0 && ((timeNow - solenoidTimer) >= valveSwitchTime)){
    // start by firing solenoid 1
    digitalWrite(SOL_1, HIGH);
    digitalWrite(SOL_2, LOW);

    // update the solenoid timer
    solenoidTimer = timeNow;

    // update the solenoid state
    solenoidState = 1;
  }

    if(solenoidState == 1 && ((timeNow - solenoidTimer) >= valveSwitchTime)){
    // start by firing solenoid 1
    digitalWrite(SOL_1, LOW);
    digitalWrite(SOL_2, HIGH);

    // update the solenoid timer
    solenoidTimer = timeNow;

    // update the solenoid state
    solenoidState = 0;
  }
}