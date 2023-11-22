#include <Arduino.h>

const byte redpin = 2;
const byte greenpin = 5;
const byte bluepin = 0;

void RGB_color(int red_light_value, int green_light_value, int blue_light_value){
  analogWrite(redpin, red_light_value);
  analogWrite(greenpin, green_light_value);
  analogWrite(bluepin, blue_light_value);
}

void rgbled(String color){
    if (color == "Red"){
        RGB_color(255, 0, 0); //Red 
    }

    if (color == "Green"){
        RGB_color(0, 255, 0); //Green
    }

    if (color == "Blue"){
        RGB_color(0, 0, 255); // Blue
    }

    if (color == "Raspberry"){
        RGB_color(255, 255, 125); // Raspberry
    }

    if (color == "Cyan"){
        RGB_color(0, 255, 255); // Cyan
    }

    if (color == "Magenta"){
        RGB_color(255, 0, 255); // Magenta
    }

    if (color == "Yellow"){
        RGB_color(255, 255, 0); // Yellow
    }

    if (color == "White"){
        RGB_color(255, 255, 255); // White
    }
}