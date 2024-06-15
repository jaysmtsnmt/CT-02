#include <Arduino.h>
#include <Servo.h>
#include "01 eyes.cpp" 
//#include "include.h"
#include "02 heart.cpp"
#include "05 servo.cpp"
#include "03 lightsensor.cpp"
//#include "hardware.cpp"
#include <Time.h>

/*
need to solve how to split string when recieve for servo data

*/

//Pins 
const byte lepin = 11; 
const byte repin = 10; 
const byte lerpin = 5;
const byte rerpin = 6;

int loopnumber = 1;
int log1;
//int log2;
int hourcount = 0;

int idletosleep = 10; //15 loops to sleep

//Heart
int heartpin = 9;

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

//Classes

void setup() {
  //starting state
  state = "sleep"; //must start with sleep or else log will freak out

  Serial.begin(9600);
  Serial.println("[EVENT] Setup started");

  pinMode(10, OUTPUT); //left eye
  pinMode(11, OUTPUT); //right eye
  pinMode(6, OUTPUT); //left eye red
  pinMode(5, OUTPUT); //right eye red
  pinMode(9, OUTPUT);

  openeyes(nrec, brightness);

  asa = swriteimmediate(90);

  Serial.println("[EVENT] Setup finished.");

}

void idle(){
  Serial.println("State: Idle ");
  //openeyes(re, le, nrec, brightness);
  beat();
  delay(random(1000, 4000)); //original 3000
  blink(blinktime, brightness);

  asa = randefaultservo(asa);
  
  //Serial.println("Servo Angle"); //debug
  //Serial.println(asa);

  beat();
  delay(random(900, 2000)); //original 2000

  /* NOT DONE WITH CODE
  if (pressed == 1){
    int currentsec = second();

      if ((second() - currentsec) > 1){
        for (pressed = 0; pressed == 0;){
        pressed = digitalRead(button);

        }
    }
  }
  */
  
  //Fall Asleep 
  if ((loopnumber - log1) > idletosleep){
    state = "sleep";
    Serial.println("[EVENT] Going to Sleep....");

    asa = swrite(asa, 98, 50);
    detatch(); //detatch servo
  }

}

void sleep(){
  Serial.println("State: Sleep");
  
  beat();
  Serial.println("[DEBUG] Heartbeat Main");
  fadeon(sleepbrightness);
  digitalWrite(rer, LOW);
  digitalWrite(ler, LOW);

  delay(1000);

  readlv1 = lightvalue();

  beat();
  fadeoff(sleepbrightness);
  delay(50);

  readlv2 = lightvalue();

  if (readlv1 < 60 || readlv2 < 60){
    state = "idle";
    log1 = loopnumber;
    Serial.print("[EVENT] Woken at loop number: ");
    Serial.println(log1);
    openeyes(nrec, brightness);
  
  }
  

  
  int nightmarechance = random(0, 1000);

  if (nightmarechance == 5){
    Serial.println("[EVENT] Nightmare detected.");

    for (int z = 0; z < nightmarechance + 5; z++){
      fadeoff(0);
      fadeon(sleepbrightness);
      delay(50);
      beat();
      beat();

      fadeoff(sleepbrightness);
      delay(60);
      beat();
    }
  }

}

void loop() {
  Serial.println(" ");
  Serial.print("Loop Number: ");
  Serial.println(loopnumber);
  Serial.print("Runtime (m): ");
  beat();
  beat();
  /*
    int x = minute();

  if (x == 59){ //FEATURE BROKEN, NEEDS FIX
     hourcount += 1;
  }

  Serial.print(hourcount);
  Serial.print("H ");
  Serial.print(x);
  Serial.println("M");

  */

  if (state == "idle"){
    idle(); //to prevent sleep log1 = loopnumber
  }

  if (state == "sleep"){
    sleep();

    int wakechance = random(0, 50); //0,50
    if (wakechance == 5){
      state = "idle";
      log1 = loopnumber;
      Serial.print("[EVENT] Waken by wakechance at loop: ");
      Serial.println(log1);
    }

  }

 loopnumber += 1;
}
