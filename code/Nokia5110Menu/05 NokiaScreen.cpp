//Include Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//Settings for Screen
bool backlight = true; 
int contrast = 50; 

Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);

void setup() {
    
    // pinMode(2, INPUT_PULLUP);
    // pinMode(1, INPUT_PULLUP);
    // pinMode(0, INPUT_PULLUP);

    pinMode(7,OUTPUT); //Backlight PIN 

    digitalWrite(7, LOW); //Turn Backlight ON 
    
    Serial.begin(9600);
    
    //Initialise display 
    display.begin();      
    display.setContrast(contrast); 
    display.clearDisplay(); 
    display.display(); //you must call this every loop
    display.invertDisplay(false);
    display.setRotation(180); //rotation function
}
