
#include <LiquidCrystal.h>

#define RED_CHANNEL 3
#define GREEN_CHANNEL 5
#define BLUE_CHANNEL 6

LiquidCrystal lcd(7,8,9,10,11,12);

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_CHANNEL, OUTPUT);
  pinMode(GREEN_CHANNEL, OUTPUT);
  pinMode(BLUE_CHANNEL, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Screen Test");
}

float x = 0.0;

void loop() {
  x += 0.01;
  
  // put your main code here, to run repeatedly:
  analogWrite(RED_CHANNEL, 80*abs(cos(x/30)));
  analogWrite(GREEN_CHANNEL, 80*abs(cos(x/10)));
  analogWrite(BLUE_CHANNEL, 80*abs(cos(x/20)));
  
}
