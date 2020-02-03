
#define SOL_A 7
#define SOL_B 8

void setup()
{
  pinMode(SOL_A, OUTPUT);
  pinMode(SOL_B, OUTPUT);
}

void loop() 
{
  digitalWrite(SOL_A, HIGH);
  delay(500);
  digitalWrite(SOL_A, LOW);
  delay(100);
  digitalWrite(SOL_B, HIGH);
  delay(500);
  digitalWrite(SOL_B, LOW);
  delay(100);
}
