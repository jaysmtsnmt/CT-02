#include <Arduino.h>
#include <Servo.h>
#define INPUT_SIZE 30

//Pins
int servopin = 8;
Servo neck;

//Settings
const int maxleftrot = 60;
const int maxrightrot = 120;
const int increment = 1; //INCREMENT OF 1 ANGLE DO NOT CHANGE

int speeddelay1 = 20; //miliseconds
int speeddelay; 

int swriteimmediate(int angle){
    neck.attach(servopin);
    delay(10);

    Serial.print("[SERVO] Angle Written: ");
    Serial.println(angle);
    neck.write(angle);
    int asa = angle;
    neck.detach();

    return asa;
}

int swrite(int asa, int angle, int servodelay){
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

void detatch(){
    neck.detach();
}

int randefaultservo(int asa){
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
    neck.detach();
  
}

