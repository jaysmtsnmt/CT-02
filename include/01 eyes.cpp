#include <Arduino.h>

int nrec = 20; //Natural Delay
int offvalue = 0;
int breathingdelay = 20; //breathing delay per +1 value of brightness (0-255 +1)
int blinktime = 200;
int brightness = 150;
int sleepbrightness = 30;

int le = 2; 
int re = 3; 
int ler = 18;
int rer = 19;

void blink(int x = blinktime, int b = brightness){ //Pass x as mils closed / Blinktime
    //Serial.println("[EXECUTE] Blink");
    pinMode(le, OUTPUT); //left eye
    pinMode(re, OUTPUT); //right eye
    //pinMode(ler, OUTPUT); //left eye red
    //pinMode(rer, OUTPUT); //right eye red

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
        analogWrite(re, b);
        delay(nrec);
        analogWrite(le, b);

        delay(x);

        analogWrite(le, offvalue);
        delay(nrec);
        analogWrite(re, offvalue);

        delay(x);

        analogWrite(le, b);
        delay(nrec);
        analogWrite(re, b);


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

void openeyes(int n = nrec, int b = brightness) {
    Serial.println("[EXECUTE] Open Eyes ");
    pinMode(le, OUTPUT); //left eye
    pinMode(re, OUTPUT); //right eye
    //pinMode(ler, OUTPUT); //left eye red
    //pinMode(rer, OUTPUT); //right eye red


    //Open Eyes
    analogWrite(le, b);
    delay(n);
    analogWrite(re, b);

}

void fadeon(int speed = breathingdelay, int maxbrightness = sleepbrightness) {
    pinMode(le, OUTPUT); //left eye
    pinMode(re, OUTPUT); //right eye
    //pinMode(ler, OUTPUT); //left eye red
    //pinMode(rer, OUTPUT); //right eye red

    for (int x = 0; x < maxbrightness; x++){
        analogWrite(re, x);
        analogWrite(le, x);
        delay(speed);
    }
}

void fadeoff(int speed = breathingdelay, int maxbrightness = sleepbrightness) {
    pinMode(le, OUTPUT); //left eye
    pinMode(re, OUTPUT); //right eye
    //pinMode(ler, OUTPUT); //left eye red
    //pinMode(rer, OUTPUT); //right eye red

    for (int x = 0; x < maxbrightness; maxbrightness--){
        analogWrite(re, maxbrightness);
        analogWrite(le, maxbrightness);
        delay(speed);
    }
}

void blueoff(){
    pinMode(le, OUTPUT); //left eye
    pinMode(re, OUTPUT); //right eye
    digitalWrite(le, LOW);
    digitalWrite(re, LOW);
}