/*
  Example for receiving
  
  https://github.com/sui77/rc-switch/
  
  If you want to visualize a telegram copy the raw data and 
  paste it into http://test.sui.li/oszi/
*/
#include <Arduino.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
String toPrint;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available()) {
    Serial.println("9000");
    // Serial.println(mySwitch.getReceivedValue());
    // Serial.println(mySwitch.getReceivedBitlength());
    // Serial.println(mySwitch.getReceivedDelay());
    // //Serial.println(mySwitch.getReceivedRawdata());
    // Serial.println(mySwitch.getReceivedProtocol());
  }
}