#include <Arduino.h>
#include <IRremote.h>

int IRpin = 7;

void setup(){
  pinMode(IRpin, INPUT);
}

void loop(){
  while (digitalRead(IRpin) == HIGH){
    // Record the pulse length in microseconds
    unsigned long pulseLength = pulseIn(IRpin, LOW);

    // Print the pulse length
    Serial.println(pulseLength);
  }
}