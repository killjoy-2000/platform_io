#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <date.cpp>

#define ssid "beast_2-4GHz"
#define password "iamthebeast"
#define NTP_SERVER "pool.ntp.org"
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN D8
// #define utcOffsetInSeconds "19800"
const int utcOffsetInSeconds = 19800;
const String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
String mins,hour;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, utcOffsetInSeconds);
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// String convertTimeToDate(time_t epochTime);

void setup()
{
  // delay(4000);
  Serial.begin(115200);
  Serial.print("Starting.....");
  delay(1000);
  Serial.println("connecting to wifi .....");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("connected");
  timeClient.begin();

  myDisplay.begin(2);

  // Set the intensity (brightness) of the display (0-15)
  // myDisplay.setZone(1, 3, 3);
  myDisplay.setIntensity(9);

  // Clear the display
  // myDisplay.displayClear();
  // myDisplay.displayZoneText(0, "hello", PA_CENTER, 10, 1000, PA_SCROLL_UP, PA_NO_EFFECT);
}

void loop()
{
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  // Serial.println("2nd line");
  Serial.println(timeClient.getEpochTime());
  Serial.println(timeClient.getDay());
  time_t epochTime = timeClient.getEpochTime();

  String weekDay = weekDays[timeClient.getDay()];
  Serial.print("Week Day: ");
  Serial.println(weekDay);
  Serial.println(f_date(epochTime));
  if(timeClient.getMinutes() < 10){
    mins = "0" + String(timeClient.getMinutes());
  }
  else if (timeClient.getMinutes() >= 10){
    mins = String(timeClient.getMinutes());
  }
  if(timeClient.getHours() < 10){
    hour = "0" + String(timeClient.getHours());
  }
  else if (timeClient.getHours() >= 10){
    hour = String(timeClient.getHours());
  }
  String time = hour + ":" + mins;
  const char *time_char = time.c_str();

  // myDisplay.setTextAlignment(PA_CENTER);
  // myDisplay.setInvert(true);
  // myDisplay.print(time);
  // myDisplay.displayText(time_char, PA_CENTER, 0, 3);
  myDisplay.setZone(0, 0, 3);
  // myDisplay.setZone(1, 0, 0);
  // myDisplay.setFont()
  myDisplay.displayZoneText(0, time_char, PA_CENTER, 10, 1000, PA_PRINT, PA_NO_EFFECT);
  // myDisplay.displayAnimate();
  // myDisplay.displayZoneText(1, "HEllo", PA_CENTER, 60, 100, PA_PRINT, PA_NO_EFFECT);

  // myDisplay.setTextEffect(1, PA_SCAN_VERT, PA_SCROLL_LEFT);

  // myDisplay.setZone(1, 0);
  // myDisplay.displayText("o", PA_CENTER, 10, 1000, PA_PRINT, PA_NO_EFFECT);
  myDisplay.displayAnimate();

  delay(1000);
}