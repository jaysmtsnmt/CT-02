#include <Arduino.h>
#include <Servo.h>

//Personal Libraries
#include "01 eyes.cpp" 
#include "02 heart.cpp"
#include "03 lightsensor.cpp"
#include "04 RGB.cpp"
#include "05 servo.cpp"
#include "06 datasplitter.cpp"

//Pins
const byte le = 8; //10
const byte re = 7; //11
const byte ler = 10;
const byte rer = 9;
int heart = 3;


//Logging and Loops
String state; 
int loopnumber = 1;
int log1;
int log2;
int hourcount = 0;
int idletosleep = 10; //15 loops to sleep
int wakecount = 0;

//Configuration for eyes
int blinktime = 200;
int brightness = 150;
int sleepbrightness = 30;

//Servo
int asa = 90; //active servo angle

//Light Sensor
int readlv1;
int readlv2;

//Python Serial communication
char* datarecieve;
//char datarecieve[25]; //max characters are 25


void setup() {
    Serial.begin(9600);
    Serial.setTimeout(1);

    Serial.println("[EVENT] Setup started");

    state = "sleep"; //must start with sleep or else log will freak out

    pinMode(le ,OUTPUT); //left eye
    pinMode(re ,OUTPUT); //right eye
    pinMode(ler, OUTPUT); //left eye red
    pinMode(rer, OUTPUT); //right eye red
    pinMode(heart, OUTPUT);
    openeyes(re, le, nrec, brightness, "1openeyes");
    asa = swriteimmediate(90);

    Serial.println("[EVENT] Setup finished.");
}


void loop() {
    while (!Serial.available());
    Serial.readString().toCharArray(datarecieve, 30);

    Serial.print("[COMMAND] ");
    Serial.println(datarecieve);

    beat(heart, datarecieve);

    blink(re, le, blinktime, brightness, datarecieve);

    fadeon(re, le, sleepbrightness, datarecieve);

    
    asa = randefaultservo(asa, datarecieve);

    asa = swrite(asa, 90, 50, datarecieve);

    asa = swriteimmediate(90, datarecieve);

    detatch(datarecieve);

}