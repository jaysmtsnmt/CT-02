#include <Arduino.h>
#include <ArduinoSTL.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>

const char* seperator = "/";
char data[] = "11/swrite/test3523454";
char* array[20];

void tokenise(char data[], char *array[20]){
    char* token = strtok(data, seperator);
    int i = 0;
    Serial.print("[DATA] "); 

    while (token != NULL){
        array[i++] = token;
        Serial.print(i-1); Serial.print("( "); Serial.print(token); Serial.print(" )"); Serial.print(" ");
        token = strtok(NULL, "/");
    }
    Serial.println();
}

void datatkn(char data[], char *array[10]){
    char* token = strtok(data, ".");
    int i = 0;
    Serial.print("[DATA] ");

    while(token != NULL){
        array[i++] = token;
        Serial.print(i-1); Serial.print("( "); Serial.print(token); Serial.print(" )"); Serial.print(" ");
        token = strtok(NULL, ".");
    }

    Serial.println();
}
