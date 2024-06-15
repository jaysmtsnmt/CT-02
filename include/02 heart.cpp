#include <Arduino.h>

int strength = 170;
int heart = 14;
int period = 50; //Miliseconds

void beat(int s = strength, int p = period){
  pinMode(heart, OUTPUT);
  Serial.println("[EXECUTE] Heart Beat");

  analogWrite(heart, s);
  delay(p);
  digitalWrite(heart, LOW);
}