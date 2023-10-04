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

const int utcOffsetInSeconds = 19800;
const String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
String mins, hour;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, utcOffsetInSeconds);
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// String convertTimeToDate(time_t epochTime);

void setup()
{
  myDisplay.begin(2);
  myDisplay.setIntensity(5);
  myDisplay.displayClear();
  myDisplay.setZone(0, 0, 3);
  myDisplay.displayZoneText(0, "Join..", PA_CENTER, 80, 800, PA_PRINT, PA_NO_EFFECT);
  myDisplay.displayAnimate();

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
  myDisplay.displayZoneText(0, "done", PA_CENTER, 80, 800, PA_PRINT, PA_NO_EFFECT);
  myDisplay.displayAnimate();
  delay(2000);
}

void loop()
{
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  // Serial.println(timeClient.getEpochTime());
  // Serial.println(timeClient.getDay());
  time_t epochTime = timeClient.getEpochTime();
  String weekDay = weekDays[timeClient.getDay()];
  Serial.print("Week Day: ");
  Serial.println(weekDay);
  Serial.println(f_date(epochTime));

  if (timeClient.getMinutes() < 10)
  {
    mins = "0" + String(timeClient.getMinutes());
  }
  else if (timeClient.getMinutes() >= 10)
  {
    mins = String(timeClient.getMinutes());
  }
  if (timeClient.getHours() < 10)
  {
    hour = "0" + String(timeClient.getHours());
  }
  else if (timeClient.getHours() >= 10)
  {
    hour = String(timeClient.getHours());
  }
  String time = hour + ":" + mins;
  const char *time_char = time.c_str();

  myDisplay.setZone(0, 0, 3);
  myDisplay.displayZoneText(0, time_char, PA_CENTER, 10, 1000, PA_PRINT, PA_NO_EFFECT);
  myDisplay.displayAnimate();

  delay(1000);
}