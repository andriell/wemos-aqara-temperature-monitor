#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
// Time Timekeeping functionality for Arduino
#include <TimeLib.h>
// ArduinoJson
#include <ArduinoJson.h>
// ESP8266 and ESP32 OLED driver for SSD1306 displays
#include<SH1106.h>

#define MAX_UL     0xFFFFFFFFUL

#define WEMOS_D0     16
#define WEMOS_D1     5
#define WEMOS_D2     4
#define WEMOS_D3     0
#define WEMOS_D4_LED 2
#define WEMOS_D5     14
#define WEMOS_D6     12
#define WEMOS_D7     13
#define WEMOS_D8     15
#define WEMOS_TX     1
#define WEMOS_RX     3

#define TFT_CS   WEMOS_D2
#define TFT_DC   WEMOS_D3
#define TFT_RST  WEMOS_D1

unsigned long mainMillis() {
  return  millis() + (MAX_UL - 30000UL);
}

void setup() {
  wemosSetup();
  ledSetup();
  displaySetup();
  wifiSetup();
  timeSetup();
  weatherSetup();
  owmSetup();
  Serial.print("Max unsigned long: ");
  Serial.println(MAX_UL);
  Serial.print("Max unsigned long + 1000: ");
  Serial.println(MAX_UL + 1000UL);
}

void loop()
{
  unsigned long startMillis = mainMillis();

  ledOff();
  wifiLoop();
  timeLoop();
  weatherLoop();
  owmLoop();
  displayLoop();

  unsigned long endMillis = mainMillis();
  unsigned long executionTime = 1000UL;
  if (endMillis < startMillis) {
    executionTime = MAX_UL - startMillis;
    executionTime += endMillis;
  } else {
    executionTime = endMillis - startMillis;
  }

  if (executionTime < 1000UL) {
    delay(1000UL - executionTime);
  }
}
