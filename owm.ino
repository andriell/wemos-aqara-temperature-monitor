// Open weather map
// https://openweathermap.org/weather-conditions

struct StructOwm {
  int id = 800;
  String main;
  String description;
  int icon = 0;
  String city;
};

#define OWM_PERIOD 307
int owmLoopCount = OWM_PERIOD;

struct StructOwm owm;

void owmSetup() {

}

void owmLoop() {
  owmLoopCount++;
  if (owmLoopCount < OWM_PERIOD) {
    return;
  }
  owmLoopCount = 0;
  
  owm.id = 0;
  owm.main = String("");
  owm.description = String("");
  owm.icon = 0;
  owm.city = String("");
  
  if ((WiFi.status() != WL_CONNECTED)) {
    return;
  }

  WiFiClient client;
  HTTPClient http;

  Serial.println("[HTTP] begin...");
  if (!http.begin(client, configOwmUrl)) {  // HTTP
    Serial.println("[HTTP} Unable to connect");
    return;
  }

  Serial.println("[HTTP] GET...");
  int httpCode = http.GET();
  if (httpCode <= 0) {
    Serial.print("[HTTP] GET... failed, error: ");
    Serial.println(http.errorToString(httpCode).c_str());
    http.end();
    return;
  }
  Serial.print("[HTTP] GET... code: ");
  Serial.println(httpCode);

  if (!(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)) {
    http.end();
    return;
  }

  String responseStr = http.getString();
  Serial.println(responseStr);
  http.end();

  DynamicJsonDocument doc(1024);

  deserializeJson(doc, responseStr);
  JsonObject responseObj = doc.as<JsonObject>();
  owm.id = responseObj["weather"][0]["id"];
  owm.main = responseObj["weather"][0]["main"].as<String>();
  owm.description = responseObj["weather"][0]["description"].as<String>();
  String icon = responseObj["weather"][0]["icon"];
  owm.icon = icon.substring(0, 2).toInt();
  owm.city = responseObj["name"].as<String>();

  printOwm(owm);
}

void printOwm(struct StructOwm owm) {
  Serial.print("OpenWeatherMap");
  Serial.print(" id: ");
  Serial.print(owm.id);
  Serial.print("; main: ");
  Serial.print(owm.main);
  Serial.print("; description: ");
  Serial.print(owm.description);
  Serial.print("; icon: ");
  Serial.print(owm.icon);
  Serial.print("; city: ");
  Serial.print(owm.city);
  Serial.println();
}

int owmIcon() {
  return owm.icon;
}

String owmCity() {
  return owm.city;
}

String owmDescription() {
  return owm.description;
}
