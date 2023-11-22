/*
millis seems to be working, test again and tidy up code?
*/

#include <Arduino.h>
#include <Servo.h>
#include <Time.h>
#include "time.h"
//#include "Time.cpp"
#include "01 eyes.cpp" 
#include "02 heart.cpp"
#include "05 servo.cpp"

const byte le = 8; //10
const byte re = 7; //11
const byte ler = 10;
const byte rer = 9;


int loopnumber = 1;
int log1;
int log2;
int hourcount = 0;
bool repeatcheck = false; 

int idletosleep = 10; //15 loops to sleep

//Heart
int heart = 3;

//Eyes
int blinktime = 200;
int brightness = 150;
int sleepbrightness = 30;

//Servo
int asa = 90; //active servo angle

//Sensor
int readlv1;
int readlv2;

int wakecount = 0;

String state; 

void setup() {
    //starting state
    state = "sleep"; //must start with sleep or else log will freak out

    Serial.begin(9600);
    Serial.println("[EVENT] Setup started");

    pinMode(le ,OUTPUT); //left eye
    pinMode(re ,OUTPUT); //right eye
    pinMode(ler, OUTPUT); //left eye red
    pinMode(rer, OUTPUT); //right eye red
    pinMode(heart, OUTPUT);

    openeyes(re, le, nrec, brightness);

    asa = swriteimmediate(90);

    Serial.println("[EVENT] Setup finished.");

}

void idle(){
    Serial.println("State: Idle ");
    //openeyes(re, le, nrec, brightness);
    beat(heart);
    unsigned long event1 = 0;

    if (repeatcheck == false){
        event1 = millis();
        repeatcheck = true;
        Serial.println(event1);
    }

    millischeck(event1, millis(), random(1000, 4000));

    if (millisbool == true){
        blink(re, le, blinktime, brightness);

        asa = randefaultservo(asa);

        //Serial.println("Servo Angle"); //debug
        //Serial.println(asa);

        beat(heart);
        delay(random(1000, 4000)); //original 3000

        delay(random(900, 2000)); //original 2000

        //Fall Asleep 
        if ((loopnumber - log1) > idletosleep){
            state = "sleep";
            Serial.println("[EVENT] Going to Sleep....");

            asa = swrite(asa, 90, 50);
            detatch(); //detatch servo
        }

        millisbool = false;
        repeatcheck = false;
    }
}


void sleep(){
    Serial.println("State: Sleep");
    
    beat(heart);
    fadeon(re, le, sleepbrightness);
    digitalWrite(rer, LOW);
    digitalWrite(ler, LOW);

    delay(1000);

    readlv1 = lightvalue();

    beat(heart);
    fadeoff(re, le, sleepbrightness);
    delay(50);

    readlv2 = lightvalue();

    if (readlv1 < 60 || readlv2 < 60){
        state = "idle";
        log1 = loopnumber;
        Serial.print("[EVENT] Woken at loop number: ");
        Serial.println(log1);
        openeyes(re, le, nrec, brightness);
    
    }
    

    int nightmarechance = random(0, 1000);

    if (nightmarechance == 5){
        Serial.println("[EVENT] Nightmare detected.");

        for (int z = 0; z < nightmarechance + 5; z++){
        fadeoff(re, le, 0);
        fadeon(rer, ler, sleepbrightness);
        delay(50);
        beat(heart);
        beat(heart);

        fadeoff(rer, ler, sleepbrightness);
        delay(60);
        beat(heart);
        }
    }

}

void loop() {
        //state = "sleep";
        Serial.println(" ");
        Serial.print("Loop Number: ");
        Serial.println(loopnumber);
        Serial.print("Runtime (m): ");

        int x = minute();

        if (x == 59){ //FEATURE BROKEN, NEEDS FIX
            hourcount += 1;
        }

        Serial.print(hourcount);
        Serial.print("H ");
        Serial.print(x);
        Serial.println("M");

        if (state == "idle"){
        idle(); //to prevent sleep log1 = loopnumber
        }

        if (state == "sleep"){
        sleep();

        int wakechance = random(0, 50);
        if (wakechance == 5){
            state = "idle";
            log1 = loopnumber;
            Serial.print("[EVENT] Waken by wakechance at loop: ");
            Serial.println(log1);
        }

    }

    loopnumber += 1;
}
