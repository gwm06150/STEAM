// Gene and John
// February 12, 2020
// Test Code for the flow controller subsystem

// Defined Pins
#define PIN_DIR  4
#define PIN_STEP 7
#define SOL_1 8 // first solenoid
#define SOL_2 9 // second solenoid

// Global variables
int counter = 0;



void setup() {
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(SOL_1, OUTPUT);
  pinMode(SOL_2, OUTPUT);
}



void loop() {
  digitalWrite(PIN_DIR, HIGH);

  for(int i = 0; i < 1600; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delay(1);
    digitalWrite(PIN_STEP, LOW);
    delay(1);
  }
  delay(1000);
  
  digitalWrite(PIN_DIR, LOW);
  
  for(int i = 0; i < 1600; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delay(1);
    digitalWrite(PIN_STEP, LOW);
    delay(1);
  }
  delay(1000);
  
}