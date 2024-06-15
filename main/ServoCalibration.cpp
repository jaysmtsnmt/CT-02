#include <Arduino.h>
#include <Servo.h>
#include <string.h>

int servopin = 9; 
int angle = 0;
Servo servo;

int getMultiple()
{
  int i = Serial.parseInt();


  // Like Serial.readString().toInt(), throw out any 
  // extra characters that arrive after the number
  // and before the timeout.
  unsigned long startTime = millis();
  while (millis() - startTime < 1000)
    Serial.read();


  return i;
}

void setup(){
    servo.attach(servopin);
    Serial.begin(9600);
    servo.write(90);
}

void loop(){
    if (Serial.available()){
        angle = Serial.readString().toInt();
        Serial.println(angle);
        servo.write(angle);
    }
}
  

