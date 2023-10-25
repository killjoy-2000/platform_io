#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial mySerial(D5, D6);

String rcv = "";
DynamicJsonDocument jsonBuffer(1024);

void setup(){
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop(){
  // Serial.println("hello world");
  while(mySerial.available()){
    Serial.print("data available  > ");
    // Serial.print(mySerial.read());
    // char d = mySerial.read();
    // jsonBuffer.add(d);
    // if (jsonBuffer.is_complete()) {
    DeserializationError error = deserializeJson(jsonBuffer, mySerial);
    if(!error){
      String message = jsonBuffer["e"];
      String msg2 = jsonBuffer["f"];
      Serial.print(message);
      Serial.print(": ");
      Serial.print(msg2);
    }
    if(error){
      Serial.print("Error parsing JSON");
    }
    // jsonBuffer.clear();
    // rcv += d;
    // if(d == '\n'){
    //   // Serial.print("rcv: ");
    //   Serial.print(rcv);
    //   rcv = "";
    // }
    // Serial.print(d);
    Serial.print(" > ");
    Serial.println(millis());
    mySerial.flush();
    delay(500);
  }
  if(!mySerial.available()){
    Serial.print("no data available > ");
    Serial.println(millis());
  }
  // delay(500);
}