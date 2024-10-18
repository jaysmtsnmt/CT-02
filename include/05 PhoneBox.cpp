#include <Arduino.h>
#include <Servo.h>

int ejectorpin = 11;
int maxangle = 110;
Servo ejector;

void eject(){
    ejector.attach(ejectorpin);
    ejector.write(maxangle);
    //delay(200);
    ejector.detach();
}

void reset(){
    ejector.attach(ejectorpin);
    ejector.write(0);
    //delay(200);
    ejector.detach();
}
