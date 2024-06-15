//Pre-Definitions
#include <Arduino.h>
#include <iostream>

//Hardware Includes
#include "01 Eyes.cpp" 
#include "02 Heart.cpp"
#include "03 Neck.cpp"
#include "04 DataHandler.cpp"

//Internal Variables/Definitions
int idletosleep = 10; //Number of idle loops till sleep
int sleeptoidle = 15; //Number of sleep loops till idle
int loopnumber; //Log Variable for Loop Number
int changestate; //Log Variable for Serial Print (during wake & sleep) & for tracking number of sleep/idle cycles
String state; //String containing the active state

const char* statecmdtag = "00";
char eyescmdtag[] = "01";
char heartcmdtag[] = "02";
char rgbcmdtag[] = "03";
char servocmdtag[] = "04";

//Settings
//Servo Settings
int defaultangle = 98; //when the face is completely facing in front
int asa = defaultangle; //active servo angle

//Functions
void idle(bool fallasleep = true, int fallasleepafter = idletosleep){
    Serial.println("[STATE] Idle Loop");

    beat();
    delay(random(1000, 4000)); //original 3000

    //Blink Eye
    blink(blinktime, brightness);

    //Move Neck
    if (random(0, 3) == 2){
        asa = randefaultservo(asa); 
    }

    beat();

    delay(random(900, 2000)); //original 2000

    //Fall Asleep after x no of loops
    if (((loopnumber - changestate) > fallasleepafter) && (fallasleep == true)){
        state = "sleep";
        changestate = loopnumber;

        Serial.println("[BOOL] Fall Asleep = True");
        Serial.println("[EVENT] Going to Sleep....");

        asa = swrite(asa, 98, 50);
        detatch(); //detatch servo
        Serial.println("[STATE]=sleep");
    }
}

void sleep(bool wake = false, int wakeafter = sleeptoidle){
    Serial.println("[STATE] Sleep Loop");
    
    beat();
    fadeon();

    delay(1000);

    beat();
    fadeoff();
    delay(50);

     /* 
     int nightmarechance = random(0, 1000);
     if (nightmarechance == 5){
        Serial.println("[EVENT] Nightmare detected.");

        for (int z = 0; z < nightmarechance + 5; z++){
            fadeoff(0);
            fadeon();
            delay(50);
            beat();
            beat();

            fadeoff();
            delay(60);
            beat();
        }
    }
     */   
    
    int wakechance = random(0, 5000); //0,50

    if (wakechance == 5){
        state = "idle";
        Serial.println("[STATE]=idle");
        changestate = loopnumber;

        Serial.print("[EVENT] Waken by wakechance at loop: ");
        Serial.println(changestate);
    }

    if (wake == true && (loopnumber - changestate) == wakeafter){
        state = "idle";
        Serial.println("[STATE]=idle");
        changestate = loopnumber;

        Serial.print("[EVENT] Waken after ");
        Serial.print(wakeafter);
        Serial.println(" sleep loops");
    }
}

//Execution
void setup() {
    state = "sleep"; 
    changestate = 1;
    loopnumber = 1;

    Serial.begin(9600);
    Serial.println("[EVENT] Setup started");

    //fadeon();
    swriteimmediate(defaultangle);

    Serial.println("[EVENT] Setup finished.");
}

void loop(){
    char rawdata[20]; //RAWDATA
    char* inputdata[20]; //processed data

    Serial.println(" ");
    Serial.print("Loop Number: ");
    Serial.println(loopnumber);
    loopnumber++;

    Serial.println("[C++] Checking for Data");
    while (Serial.available() > 0) { //Check for python data
        Serial.readString().toCharArray(rawdata, 20);
        Serial.println("[C++] Data Recieved");
        tokenise(rawdata, inputdata); //raw data is seperated and stored as inputdata
        //Serial.print("[BUG] state "); Serial.println(inputdata[0]);

        if (strcmp(inputdata[0], statecmdtag) == 0){
            Serial.print("[STATE] Changing to: "); Serial.println(inputdata[1]);
            state = inputdata[1];
            //Serial.print("[STATE]="); Serial.println(state);
        }
    }   

    if (state == "idle"){
        idle(); 
    }

    if (state == "deepsleep"){
        //asa = swrite(asa, 98, 50);
        //detatch();
        blueoff(); //off all lights
        beat(130); //heartbeat
        delay(60000); //minute interval

    }

    if (state == "sleep"){
        sleep();
    }
}