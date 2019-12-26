void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) {
    int in = Serial.read();

    digitalWrite(LED_BUILTIN, in == 1 ? HIGH : LOW);
    
  }
}
