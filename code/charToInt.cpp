#include <Arduino.h>
#include <cmath>

// char ss[20] = "4023";

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

// void setup(){
//     Serial.begin(9600);
//     Serial.println("Begin");
//     Serial.println(charToInt(ss));
// }

// void loop(){

// }
