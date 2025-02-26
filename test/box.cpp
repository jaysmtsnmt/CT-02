#include <Arduino.h>
#include <Servo.h>

int servopin = 11;
Servo box;
char data;

void setup(){
    Serial.begin(9600);
    box.attach(servopin);
    box.write(0);
    box.detach();
}

void loop(){
    if (Serial.available() > 0){
        data = Serial.read();
        Serial.flush(); 

        if (data == 'y'){
            box.attach(servopin);
            box.write(100);
            delay(2000);
            box.write(0);
            
            box.detach();

        }
    }
}
