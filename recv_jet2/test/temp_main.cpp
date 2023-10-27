#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(D5, D6);

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

String rcv = "";
DynamicJsonDocument jsonBuffer(1024);

void setup(){
  Serial.begin(115200);
  mySerial.begin(115200);
  lcd.init();
  lcd.backlight();
  while(!mySerial.available()){
    lcd.setCursor(0, 0);
    lcd.print("Welcome");
    lcd.setCursor(0, 1);
    lcd.print("FOLD");
  }
  if(mySerial.available()){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Boot Successful");
    lcd.setCursor(0, 1);
    lcd.print("press M to start");
  }
  // delay(5000);
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
      String message = jsonBuffer["ip"];
      String msg2 = jsonBuffer["time"];
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
    lcd.setCursor(0, 0);
    lcd.print("Problem with coms");
    lcd.setCursor(0, 1);
    lcd.print("XXXX0000XXXX");
  }
  // delay(500);
}