#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

#define A D8
#define M D7
#define data_show 2
#define disp_delay 500

SoftwareSerial mySerial(D5, D6);

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

DynamicJsonDocument jsonBuffer(1024);
unsigned long long curr_time_t1 = 0, curr_time_A = 0, curr_time_err = 0, curr_time_disp = 0;
int count_A = 0;
String ip_d, time_d;

void setup()
{
    Serial.begin(115200);
    mySerial.begin(115200);
    lcd.init();
    lcd.backlight();

    while (!mySerial.available())
    {
        lcd.setCursor(0, 0);
        lcd.print("Welcome");
        lcd.setCursor(0, 1);
        lcd.print("FOLD");
    }
    if (mySerial.available())
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Boot Successful");
        lcd.setCursor(0, 1);
        lcd.print("press M to start");
        while (digitalRead(M) != HIGH)
        {
            delay(100);
        }
    }
}

void loop()
{
    if (millis() - curr_time_t1 >= 100)
    {
        if (mySerial.available())
        {
            Serial.print("data available  > ");
            DeserializationError error = deserializeJson(jsonBuffer, mySerial);
            if (!error)
            {
                String ip = jsonBuffer["ip"];
                ip_d = ip;
                String time = jsonBuffer["time"];
                time_d = time;
            }
            if (error)
            {
                Serial.print("Error parsing JSON");
            }
            Serial.print(" > ");
            Serial.println(millis());
            mySerial.flush();
        }
        if (!mySerial.available())
        {
            Serial.print("no data available > ");
            Serial.println(millis());
            // lcd.clear();
            // lcd.setCursor(0, 0);
            // lcd.print("Problem with coms");
            // lcd.setCursor(0, 1);
            // lcd.print("XXXX0000XXXX");
        }
        curr_time_t1 = millis();
    }

    if (digitalRead(A) == HIGH)
    {
        if (millis() - curr_time_A >= 300)
        {
            // int intency = myDisplay.getIntensity();
            count_A++;
            if (count_A == data_show)
            {
                count_A = 0;
            }
            // myDisplay.setIntensity(count_bright);
            curr_time_A = millis();
        }
    }
    switch (count_A)
    {
    case 0:
        if (millis() - curr_time_disp >= disp_delay)
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("IP -->");
            lcd.setCursor(0, 1);
            lcd.print(ip_d);
            curr_time_disp = millis();
        }
        break;
    case 1:
        if (millis() - curr_time_disp >= disp_delay)
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TIME -->");
            lcd.setCursor(0, 1);
            lcd.print(time_d);
            curr_time_disp = millis();
        }
        break;

    default:
        break;
    }
}