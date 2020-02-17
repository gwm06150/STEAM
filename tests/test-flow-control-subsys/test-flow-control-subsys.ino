
#define PIN_DIR  4
#define PIN_STEP 7


void setup() {
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);
}

void loop() {
  digitalWrite(PIN_DIR, HIGH);

  for(int i = 0; i < 1200; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delay(10);
    digitalWrite(PIN_STEP, LOW);
    delay(10);
  }
  delay(1000);
  
  digitalWrite(PIN_DIR, LOW);
  
  for(int i = 0; i < 1200; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delay(10);
    digitalWrite(PIN_STEP, LOW);
    delay(10);
  }
  delay(1000);
  
}