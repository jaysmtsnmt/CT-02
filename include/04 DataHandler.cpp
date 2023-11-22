//Library Includes
#include <Arduino.h>

//Hardware Includes

//Settings
const char* seperator = "/";
const int MAX_ARRAYSTORE = 15;
const int MAX_CHARACTERS = 30;

//Pre-Definintions
char empty[MAX_CHARACTERS] = "EMPTY";
char data[MAX_CHARACTERS];
char emptydump[MAX_CHARACTERS] = "\0";
char* emptyarray[MAX_ARRAYSTORE];
char* dataarray[MAX_ARRAYSTORE];

//Tags
char eyescmdtag[] = "01";
char heartcmdtag[] = "02";
char servocmdtag[] = "05";

//Debug
//char data[] = "11/swrite/test3523454";
//char* array1[20];


//Functions
void tokenise(char data[], char *array[MAX_ARRAYSTORE]){ //Tokenise data into char array
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

bool fetchreturn(char databytes[MAX_CHARACTERS]){ //Fetch new data from Serial Port & Return as variable (databytes)
    if (Serial.available() > 0) {
        Serial.println("[FETCH] New Data Fetched.");
        Serial.readString().toCharArray(databytes, MAX_CHARACTERS);
        return true;
    } 

    else {
        Serial.println("[FETCH] No new data fetched.");
        return false;
    }
}

void clearstring(char dump[MAX_CHARACTERS]){
    dump = emptydump;
}

void cleararray(char* arraydump[MAX_ARRAYSTORE]){
    arraydump = emptyarray;
}

