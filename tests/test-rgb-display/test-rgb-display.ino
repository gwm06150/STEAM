#define RED_CHANNEL 3
#define GREEN_CHANNEL 5
#define BLUE_CHANNEL 6

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_CHANNEL, OUTPUT);
  pinMode(GREEN_CHANNEL, OUTPUT);
  pinMode(BLUE_CHANNEL, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(RED_CHANNEL, 20);
}
