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
#define change_sw D2
#define bright_sw D1

const int utcOffsetInSeconds = 19800;
const String weekDays[7] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"};
// const String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
String mins, hour, time_show, weekDay, date_disp, show_2nd, show_3rd, last_year;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, utcOffsetInSeconds);
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int count1 = 0, count_bright = 5;

unsigned long long curr_time = 0, curr_time_Time = 0, curr_time_count = 0, curr_time_bright = 0;
char *time_char;

void setup()
{
  myDisplay.begin(2);
  myDisplay.setIntensity(count_bright);
  myDisplay.displayClear();
  myDisplay.setZone(0, 0, 3);
  myDisplay.displayZoneText(0, "WiFi...", PA_CENTER, 80, 800, PA_PRINT, PA_NO_EFFECT);
  myDisplay.displayAnimate();

  pinMode(change_sw, INPUT);
  pinMode(bright_sw, INPUT);

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

  if (digitalRead(change_sw) == HIGH)
  {
    if (millis() - curr_time_count >= 300)
    {
      count1++;
      if (count1 == 4)
      {
        count1 = 0;
      }
      curr_time_count = millis();
      curr_time = millis();
    }
  }

  if (digitalRead(bright_sw) == HIGH)
  {
    if (millis() - curr_time_bright >= 300)
    {
      // int intency = myDisplay.getIntensity();
      count_bright++;
      if (count_bright == 16)
      {
        count_bright = 0;
      }
      myDisplay.setIntensity(count_bright);
      curr_time_bright = millis();
    }
  }

  if (millis() - curr_time_Time >= 500)
  {
    // Serial.println(timeClient.getFormattedTime());
    time_t epochTime = timeClient.getEpochTime();
    weekDay = weekDays[timeClient.getDay()];
    Serial.print("Week Day: ");
    Serial.println(weekDay);
    date_disp = f_date(epochTime);
    Serial.println(date_disp);
    int len = date_disp.length();
    last_year = date_disp.substring(len - 4);
    // show_2nd = weekDay + "|" + last_year;
    Serial.print("count1: ");
    Serial.println(count1);
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
    curr_time_Time = millis();
    time_show = hour + ":" + mins;
  }

  if (count1 > 0 && (millis() - curr_time >= 5000))
  {
    count1 = 0;
  }

  if (count1 == 0)
  {
    myDisplay.setZone(0, 0, 3);
    const char *time_char = time_show.c_str();
    myDisplay.displayZoneText(0, time_char, PA_CENTER, 10, 1000, PA_PRINT, PA_NO_EFFECT);
    myDisplay.displayAnimate();
  }
  else if (count1 == 1)
  {
    myDisplay.setZone(0, 0, 3);
    const char *weekDay_char = weekDay.c_str();
    myDisplay.displayZoneText(0, weekDay_char, PA_CENTER, 10, 1000, PA_PRINT, PA_NO_EFFECT);
    myDisplay.displayAnimate();
  }
  else if (count1 == 2)
  {
    myDisplay.setZone(0, 0, 3);
    String temp = date_disp.substring(0, 5);
    const char *date_disp_char = temp.c_str();
    myDisplay.displayZoneText(0, date_disp_char, PA_CENTER, 10, 1000, PA_PRINT, PA_NO_EFFECT);
    myDisplay.displayAnimate();
  }

  else if (count1 == 3)
  {
    myDisplay.setZone(0, 0, 3);
    const char *last_year_char = last_year.c_str();
    myDisplay.displayZoneText(0, last_year_char, PA_CENTER, 10, 1000, PA_PRINT, PA_NO_EFFECT);
    myDisplay.displayAnimate();
  }
}