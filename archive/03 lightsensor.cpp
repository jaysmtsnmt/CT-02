#include <Arduino.h>

const byte lightsensor = A0;
char s[15];

int lightvalue(){
    Serial.begin(9600);
    pinMode(lightsensor, INPUT);

    int lightv = analogRead(lightsensor);
    snprintf(s, sizeof(s), "01.%i", lightv);

    //Serial.println(lightv);

    return lightv;

}
