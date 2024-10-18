//Pre-Definitions
#include <Arduino.h>
#include <iostream>

//Hardware Includes
#include "01 Eyes.cpp" 
#include "02 Heart.cpp"
#include "03 Neck.cpp"
#include "04 DataHandler.cpp"
#include "05 PhoneBox.cpp"

//Internal Variables/Definitions
int idletosleep = 10; //Number of idle loops till sleep
int sleeptoidle = 15; //Number of sleep loops till idle
int loopnumber; //Log Variable for Loop Number
int changestate; //Log Variable for Serial Print (during wake & sleep) & for tracking number of sleep/idle cycles
String state; //String containing the active state

//Serial Communication Identifiers
const char* statecmdtag = "00";
const char* trackingcmdtag = "09";
const char* neckcmdtag = "03";
const char* phoneboxcmdtag = "05";
// const char* neckcmdtag_offset = "soffset";
const char* neckcmdtag_swrite = "sw";
const char* phoneboxcmdtag_eject = "eject";
// const char* neckcmdtag_swriteimmediate = "swi";

//Settings
//Servo Settings
int defaultangle = 98; //when the face is completely facing in front
int asa = defaultangle; //active servo angle (internal)
//Face Tracking Settings (Awake)
int face_x = 0; //Pixel value for face tracking (internal)
int tracking_speed = 14; //Milliseconds 
int error_allowance = 2; //Degree (if it is n degrees away from centre)

int charToInt(char *string){
    int i = 0;
    int x = 0;
    int result = 0;

    while (string[i] != NULL){
        x++;
        i++;
    }
    x = x-1;

    for (int n=x; n >= 0; n = (n-1)){
        //Serial.println();
        // Serial.println(string[n]);
        int number = (int)string[n] - 48;
        //Serial.println(number);
        //Serial.println(ceil(pow(10,x-n)));
        result = result + (number * ceil(pow(10, x-n)));
        //Serial.println(number*ceil(pow(10,x-n)));
        //Serial.println(result);
    }

    return result;
}

void awake(){
    unsigned long start_time = 0;
    unsigned long currentMillis = 0;
    unsigned long reset_eject = 0; //time at which phone was last ejected (if = 0, means that phone has not been ejected)

    start_time = millis();
    openeyes();

    while (state == "awake"){
        currentMillis = millis();

        //Blinking
        if ((currentMillis-start_time) > random(2500, 5000)){
            blink();
            start_time = millis();
        }

        if ((reset_eject != 0) and (currentMillis-reset_eject > 3000)){ //If the phone was ejected, but ejector has not reset, and if it has been 3000 milliseconds, reset the ejector.
            reset_eject = 0;
            reset();
        }
        
        //Face Tracking
        char rawdata[20]; //RAWDATA
        char* inputdata[20]; //processed data   

        while (Serial.available() > 0) { //Check for python data
            Serial.readStringUntil('\n').toCharArray(rawdata, 20);
            tokenise(rawdata, inputdata); //raw data is seperated and stored as inputdata

            //Processing pixel data
            if (strcmp(inputdata[0], trackingcmdtag) == 0){ //Detect instruction to track face
                face_x = charToInt(inputdata[1]);
                int servoCorr = round((face_x - 640)/25.6);
                int finalAngle = (-1*servoCorr)+asa;

                if ((finalAngle <= 180) && (finalAngle >= 0)){
                    // if (abs(servoCorr) > error_allowance){
                    //     asa = swrite(asa, finalAngle, tracking_speed);
                    //     //asa = swriteimmediate(finalAngle);
                    // } #no need for this as there is already processing for pixel correction allowance in python

                    asa = swrite(asa, finalAngle, tracking_speed);
                }  
            }

            if (strcmp(inputdata[0], statecmdtag) == 0){ //Detect instruction to change state
                Serial.print("[STATE] Changing to: "); Serial.println(inputdata[1]);
                state = inputdata[1];
                changestate = loopnumber;
            }

            if (strcmp(inputdata[0], neckcmdtag) == 0){ //Detect instruction to rotate servo
                if (strcmp(inputdata[1], neckcmdtag_swrite) == 0){
                    int ang = charToInt(inputdata[2]);
                    //int servdelay = charToInt(inputdata[3]);
                    asa = swrite(asa, ang, 50);
                }
            }

            if (strcmp(inputdata[0], phoneboxcmdtag) == 0){ //Detect instruction to eject phone from box
                if (strcmp(inputdata[1], phoneboxcmdtag_eject) == 0){
                    eject();
                    reset_eject = millis(); //Reset eject becomes non-zero.
                }
            }

        }   
        Serial.flush();  //Clear serial!!
    }
}

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
    Serial.println("[CT-02] Setup started");

    //fadeon();
    swriteimmediate(defaultangle);
    Serial.println("[CT-02] Setup finished.");
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
        Serial.readStringUntil('\n').toCharArray(rawdata, 20);
        Serial.println("[C++] Data Recieved");
        tokenise(rawdata, inputdata); //raw data is seperated and stored as inputdata

        if (strcmp(inputdata[0], neckcmdtag) == 0){
            if (strcmp(inputdata[1], neckcmdtag_swrite) == 0){
                int ang = charToInt(inputdata[2]);
                int servdelay = charToInt(inputdata[3]);
                asa = swrite(asa, ang, servdelay);
            }
        }

        if (strcmp(inputdata[0], statecmdtag) == 0){
            Serial.print("[STATE] Changing to: "); Serial.println(inputdata[1]);
            state = inputdata[1];
            changestate = loopnumber;
            //Serial.print("[STATE]="); Serial.println(state);
        }
    }   

    if (state == "idle"){
        idle(); 
    }

    else if (state == "deepsleep"){
        //asa = swrite(asa, 98, 50);
        //detatch();
        blueoff(); //off all lights
        beat(130); //heartbeat
        delay(60000); //minute interval

    }

    else if (state == "awake"){
        awake();
    }

    else { //default to sleep
        //awake();
        sleep();
    }


}