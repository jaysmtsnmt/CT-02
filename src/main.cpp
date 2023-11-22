//Pre-Definitions
#include <Arduino.h>

//Hardware Includes
#include "01 eyes.cpp" 
#include "02 heart.cpp"
#include "05 servo.cpp"

//Internal Variables/Definitions
int idletosleep = 10; //Number of idle loops till sleep
int sleeptoidle = 15; //Number of sleep loops till idle
int loopnumber; //Log Variable for Loop Number
int changestate; //Log Variable for Serial Print (during wake & sleep) & for tracking number of sleep/idle cycles
String state; //String containing the active state


//Settings
//Servo Settings
int defaultangle = 98; //when the face is completely facing in front
int asa = defaultangle; //active servo angle

//Functions
void idle(bool fallasleep = true, int fallasleepafter = idletosleep){
    Serial.println("State: Idle ");

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
    }
}

void sleep(bool wake = false, int wakeafter = sleeptoidle){
    Serial.println("State: Sleep");
    
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
    
    int wakechance = random(0, 50); //0,50
    if (wakechance == 5){
        state = "idle";
        changestate = loopnumber;

        Serial.print("[EVENT] Waken by wakechance at loop: ");
        Serial.println(changestate);
    }

    if (wake == true && (loopnumber - changestate) == wakeafter){
        state = "idle";
        changestate = loopnumber;

        Serial.print("[EVENT] Waken after ");
        Serial.print(wakeafter);
        Serial.println(" sleep loops");
    }
}

//Execution
void setup() {
    state = "sleep"; //must start with sleep or else log will freak out
    changestate = 1;
    loopnumber = 1;

    Serial.begin(9600);
    Serial.println("[EVENT] Setup started");

    fadeon();

    Serial.println("[EVENT] Setup finished.");

}

void loop(){
    Serial.println(" ");
    Serial.print("Loop Number: ");
    Serial.println(loopnumber);
    loopnumber++;

    if (state == "idle"){
        idle(); 
    }

    if (state == "sleep"){
        sleep();
    }
}