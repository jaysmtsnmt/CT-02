#include <Arduino.h>
#include <06 datasplitter.cpp>
#include <Servo.h>
#include <SevSeg.h>

const int maxdataallowed = 20;

char eyescmdtag[] = "01";
char heartcmdtag[] = "02";
char rgbcmdtag[] = "03";
char servocmdtag[] = "04";
char segmentcmdtag[] = "05";

char lsensorcmdtag[] = "11";

//-----------------PINS-----------------
//Servo
int servopin = 12;

//Eyes
const byte le = 8; //10
const byte re = 7; //11
const byte ler = 10;
const byte rer = 9;

//Heart
int heartpin = 3;

//Lightsensor
const byte lightsensorpin = A0;

//RGB 
const byte redpin = 2;
const byte greenpin = 5;
const byte bluepin = 0;

//Segment Display
SevSeg display;

byte numDigits = 4; 
byte digitPins[] = {27, 29, 31, 33};
byte segmentPins[] = {6, 53, 52, 51, 50, 49, 48, 35};
byte resistorsOnSegments = true;
byte hardwareConfig = COMMON_CATHODE; 

int time; 
int hours;
int minutes; 

class eyes{
    private: 
    int nrec = 20; //Natural Delay
    int offvalue = 0;
    int breathingdelay = 20; //breathing delay per cycle
    int x = 200; //Blinktime
    int brightness = 150;
    int sleepbrightness = 30;

    public:

    //Blink must be executed with eyes ON already.
    void blink(char* inputdata[20]){ //Pass x as mils closed / Blinktime
        Serial.println("[EXECUTE] Blink");
        int choice = random(0, 3);

        if (choice == 1){
        /*
            //Close Eyes
        digitalWrite(le, LOW);
        delay(nrec);
        digitalWrite(re, LOW);

        delay(x);

        //Open Eyes
        digitalWrite(re, HIGH);
        delay(nrec);
        digitalWrite(le, HIGH);

        delay(x);

        digitalWrite(le, LOW);
        delay(nrec);
        digitalWrite(re, LOW);

        delay(x);

        digitalWrite(le, HIGH);
        delay(nrec);
        digitalWrite(re, HIGH);
        */
            //Close Eyes
            analogWrite(le, offvalue);
            delay(nrec);
            analogWrite(re, offvalue);

            delay(x);

            //Open Eyes
            analogWrite(re, brightness);
            delay(nrec);
            analogWrite(le, brightness);

            delay(x);

            analogWrite(le, offvalue);
            delay(nrec);
            analogWrite(re, offvalue);

            delay(x);

            analogWrite(le, brightness);
            delay(nrec);
            analogWrite(re, brightness);

        }

        else {

            //Close Eyes
            analogWrite(le, offvalue);
            delay(nrec);
            analogWrite(re, offvalue);

            delay(x);

            //Open Eyes
            analogWrite(re, brightness);
            delay(nrec);
            analogWrite(le, brightness);    
        }
        
    }

    void openeyes(char* inputdata[20]) {
        Serial.println("[EXECUTE] Open Eyes ");

        //Open Eyes
        analogWrite(le, brightness);
        delay(nrec);
        analogWrite(re, brightness);

    }

    void fadeon(char* inputdata[20]) {

        for (int x = 0; x < sleepbrightness; x++){
            analogWrite(re, x);
            analogWrite(le, x);
            delay(breathingdelay);
            
        }
    }

    void fadeoff(char* inputdata[20]) {
        
        for (int x = 0; x < sleepbrightness; sleepbrightness--){
            analogWrite(re, sleepbrightness);
            analogWrite(le, sleepbrightness);
            delay(breathingdelay);
        }
    }

};

class h{ //for some reason i cant name this class heart
    private: 
    int strength = 200;
    int period = 50; //Miliseconds

    public:
    void beat(char* inputdata[20]){
        analogWrite(heartpin, strength);
        delay(period);
        digitalWrite(heartpin, LOW);
    }
};

class lightsensor{
    private:

    public:
    int lightvalue(char *inputdata[20], int lightv){
        pinMode(lightsensorpin, INPUT);
        lightv = analogRead(lightsensorpin);
        //snprintf(s, sizeof(s), "01.%i", lightv);
        char temp[10];
        sprintf(temp, "!/11/%i", lightv);
        Serial.println(temp);
        
        return lightv;
    }

};

class rgblight{
    private:

    void privrgbhex(int red_light_value, int green_light_value, int blue_light_value){ //used only for rgbstring
        analogWrite(redpin, red_light_value);
        analogWrite(greenpin, green_light_value);
        analogWrite(bluepin, blue_light_value);
    }

    public:
    void rgbhex(char *inputdata[20]){ //Example: 03/rgbhex/255/65/255 (r,g,b format)
        int red_light_value = int(inputdata[2]);
        int green_light_value = int(inputdata[3]);
        int blue_light_value = int(inputdata[4]);

        analogWrite(redpin, red_light_value);
        analogWrite(greenpin, green_light_value);
        analogWrite(bluepin, blue_light_value);
    }

    void rgbstring(char* inputdata[20]){ //Lowercase only

        char* color = inputdata[2]; //Example: 03/rgbstring/red
        if (strcmp(color, "red") == 0){
            privrgbhex(255, 0, 0); //Red 
        }

        if (strcmp(color, "green") == 0){
            privrgbhex(0, 255, 0); //Green
        }

        if (strcmp(color, "blue") == 0){
            privrgbhex(0, 0, 255); // Blue
        }

        if (strcmp(color, "raspberry") == 0){
            privrgbhex(255, 255, 125); // Raspberry
        }

        if (strcmp(color, "cyan") == 0){
            privrgbhex(0, 255, 255); // Cyan
        }

        if (strcmp(color, "magenta") == 0){
            privrgbhex(255, 0, 255); // Magenta
        }

        if (strcmp(color, "yellow") == 0){
            privrgbhex(255, 255, 0); // Yellow
        }

        if (strcmp(color, "white") == 0){
            privrgbhex(255, 255, 255); // White
        }

        else {
            Serial.println("[RGB] Invalid Write Color.");
        }
    }
};

class servo{

    private:
    #define INPUT_SIZE 30
    int maxleftrot = 60;
    int maxrightrot = 120;

    int speeddelay1 = 20; //miliseconds
    int speeddelay; 
    int increment = 1; //INCREMENT OF 1 ANGLE DO NOT CHANGE

    Servo neck;

    public:
    int swriteimmediate(char* inputdata[20], int asa){ //04/servim/20 (tag/servofunc/angle)
        neck.attach(servopin);

        int angle = int(inputdata[2]);
        Serial.print("[SERVO] Angle Written: ");
        Serial.println(angle);

        neck.write(angle);

        asa = angle;

        neck.detach();
        return asa;
    }

    int swrite(char* inputdata[20], int asa){ //(ASA IS SEPERATE AND KEPT ON HARDWARD SIDE) Example 04/swrite/angle/servodelay
        int angle = int(inputdata[2]);
        int servodelay = int(inputdata[3]);

        Serial.begin(9600);
        neck.attach(servopin);

        Serial.print("[SERVO] Start Angle: ");
        Serial.println(asa);

        int wangle = angle - asa;

        //Serial.print("[SERVO] Servo must travel by: ");
        //Serial.println(wangle);

        if (wangle > 0){ //  0    90 >  180 //POSITIVE correction

            for (int x = 0; x < wangle; x++)
            {
                asa += increment;

                //For seeing Writen angles one by one
                //Serial.print("[SERVO] Servo Write: ");
                //Serial.println(int(asa));

                neck.write(int(asa));

                // x += 1;
                delay(servodelay);
            }
        }

        if (wangle < 0){ //Negative correction angle | 0  < 90   180

            wangle = -wangle; //negative to postive

            for (int x = 0; x < wangle; x++){
                asa -= increment;

                //For seeing writen angles one by one
                //Serial.print("[SERVO] Servo Write: ");
                //Serial.println(asa);

                neck.write(int(asa));

                //x += 1;
                delay(servodelay);

            }
        }


        Serial.print("[SERVO] End Angle: ");
        Serial.println(asa);

        neck.detach(); //detatch servo

        return asa;
    }

    void detach(){
        neck.detach();
    }

    int randefaultservo(char *inputdata[20], int asa){
        Serial.begin(9600);
        neck.attach(servopin);

        Serial.print("[SERVO] Start Angle: ");
        Serial.println(asa);

        neck.write(asa);
                
        int angle = random(maxleftrot, maxrightrot);
        //Serial.print("[SERVO] Random Angle: ");
        //Serial.println(angle);

        speeddelay = random(speeddelay1 - 10, speeddelay1); //Randomness for speed (natural)
        //Serial.print("[SERVO] Speed to target angle: ");
        //Serial.println(speeddelay);


        int wangle = angle - asa;

        //Serial.print("[SERVO] Servo must travel by: ");
        //Serial.println(wangle);

        if (wangle > 0){ //  0    90 >  180 //POSITIVE correction

            for (int x = 0; x < wangle; x++)
            {
                asa += increment;

                //For seeing Writen angles one by one
                //Serial.print("[SERVO] Servo Write: ");
                //Serial.println(int(asa));

                neck.write(int(asa));

                // x += 1;
                delay(speeddelay);
            }
        }

        if (wangle < 0){ //Negative correction angle | 0  < 90   180

            wangle = -wangle; //negative to postive

            for (int x = 0; x < wangle; x++){
                asa -= increment;

                //For seeing writen angles one by one
                //Serial.print("[SERVO] Servo Write: ");
                //Serial.println(asa);

                neck.write(int(asa));

                //x += 1;
                delay(speeddelay);

            }
        }

        Serial.print("[SERVO] End Angle: ");
        Serial.println(asa);
        return asa;
    }
};

class segmentdisplay{
    public:
    void setDigits(char* inputdata[20]){ //ONLY 4 DIGITS 05/4digits/1234
        
        int digits = int(inputdata[2]);
        Serial.print("[EXECUTE] 4 Digits:  "); Serial.print(inputdata[2]); Serial.print(", "); Serial.print(digits); Serial.println();
        for (int i = 0; i != 4; i++){
            display.setNumber(1234);
        }
    }
};

int run(char* inputdata[20], int returndata = 0, bool returnvalue = false){ //returndata (asa) is optional
    eyes eyes;
    h heart;
    rgblight rgb;
    servo s;
    lightsensor ls;
    segmentdisplay sd;

    char* commandtype = inputdata[0];
    char* prompt = inputdata[1];
    
    //Light Sensor 11
    if ((strcmp(commandtype, lsensorcmdtag) == 0) && (strcmp(prompt, "read") == 0)){
        ls.lightvalue(inputdata, returndata);
    }

    //EYES 01
    if ((strcmp(commandtype, eyescmdtag) == 0) && (strcmp(prompt, "blink") == 0)){
        eyes.blink(inputdata);
    }

    if ((strcmp(commandtype, eyescmdtag) == 0) && (strcmp(prompt, "openeyes") == 0)){
        eyes.openeyes(inputdata);
    }

    if ((strcmp(commandtype, eyescmdtag) == 0) && (strcmp(prompt, "fadeon") == 0)){
        eyes.fadeon(inputdata);
    }

    if ((strcmp(commandtype, eyescmdtag) == 0) && (strcmp(prompt, "fadeoff") == 0)){
        eyes.fadeoff(inputdata);
    }

    //HEART 02
    if ((strcmp(commandtype, heartcmdtag) == 0) && (strcmp(prompt, "beat") == 0)){
        heart.beat(inputdata);
    } 

    //RGB Lights 03
    if ((strcmp(commandtype, rgbcmdtag) == 0) && (strcmp(prompt, "rgbstring") == 0)){
        rgb.rgbstring(inputdata); //Example: 03/rgbstring/red
    }

    if ((strcmp(commandtype, rgbcmdtag) == 0) && (strcmp(prompt, "privrgbhex") == 0)){
        rgb.rgbhex(inputdata);
    } 

    //Servo 04 (!!! ASA IS KEPT ON THE ARDUINO SIDE)
    if ((strcmp(commandtype, servocmdtag) == 0) && (strcmp(prompt, "servim") == 0)){ //swrite immediate
        s.swriteimmediate(inputdata, returndata);
    } 

    if ((strcmp(commandtype, servocmdtag) == 0) && (strcmp(prompt, "servwr") == 0)){ //swrite
        s.swrite(inputdata, returndata);
    } 

    if ((strcmp(commandtype, servocmdtag) == 0) && (strcmp(prompt, "servran") == 0)){ //swrite random
        returndata = s.randefaultservo(inputdata, returndata);
    } 

    if ((strcmp(commandtype, servocmdtag) == 0) && (strcmp(prompt, "detach") == 0)){ //detach
        s.detach();
    } 

    //Segment Display
    if ((strcmp(commandtype, segmentcmdtag) == 0) && (strcmp(prompt, "4digits") == 0)){
        sd.setDigits(inputdata);
    }

    return returndata;

}

void setup(){
    Serial.begin(9600);

    display.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
    display.setBrightness(90);

    //Serial.print(inputdata[0]); Serial.print(" "); Serial.print(inputdata[1]);
}

void test(){
    /*
    tokenise(data, inputdata);

    asa = run(inputdata, asa);
    Serial.print("Debug: "); Serial.println(asa);
    delay(3000);
    Serial.println();

    tokenise(data2, inputdata2);
    run(inputdata2);
    Serial.println();

    */

}

int asa = 90;

//char data[] = "01/blink";
//char data2[] = "05/4digits/1234";

void loop(){
    char* inputdata[20];
    char rawdata[20];
    bool commandrecieved = false;

    while (Serial.available() > 0) {
        Serial.readString().toCharArray(rawdata, 20);
        Serial.print("[R] "); Serial.println(rawdata);
        commandrecieved = true;

    }   

    if (commandrecieved){
        display.refreshDisplay();
        tokenise(rawdata, inputdata); //Data stored as inputdata

        run(inputdata, asa);
        //print(asa);

        commandrecieved = false;
    }

} 