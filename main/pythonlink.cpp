#include <Arduino.h>
#include "04 DataHandler.cpp"


void setup(){
    Serial.begin(9600);

    //display.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
    //display.setBrightness(90);

    //Serial.print(inputdata[0]); Serial.print(" "); Serial.print(inputdata[1]);
}

void loop(){
    char rawdata[20]; //RAWDATA
    char* inputdata[20]; //processed data

    while (Serial.available() > 0) {
        Serial.readString().toCharArray(rawdata, 20);
        Serial.print("[R] "); Serial.println(rawdata);
        tokenise(rawdata, inputdata); //raw data is seperated and stored as inputdata

    }   
} 