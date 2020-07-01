#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
// ArduinoJson
#include <ArduinoJson.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
// ESP8266 and ESP32 OLED driver for SSD1306 displays
#include<SH1106.h>



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

void setup() {
  wemosSetup();
  ledSetup();
  displaySetup();
  wifiSetup();
  weatherSetup();
  owmSetup();
}

void loop()
{
  ledOff();
  weatherLoop();
  owmLoop();
  displayLoop();
  delay(30 * 60 * 1000);
}
