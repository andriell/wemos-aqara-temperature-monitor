// Open weather map
// https://openweathermap.org/weather-conditions


struct StructOwm {
  int id = 800;
  const char* main = "Clear";
  const char* description = "clear sky";
  int icon = 1;
  const char* city = "City";
};


struct StructOwm owm;

void owmSetup() {

}

void owmLoop() {
  owm.id = 0;
  owm.main = "";
  owm.description = "";
  owm.icon = 0;
  owm.city = "";
  
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
  JsonObject responceObj = doc.as<JsonObject>();
  owm.id = responceObj["weather"][0]["id"];
  owm.main = responceObj["weather"][0]["main"];
  owm.description = responceObj["weather"][0]["description"];
  String icon = responceObj["weather"][0]["icon"];
  owm.icon = icon.substring(0, 2).toInt();
  owm.city = responceObj["name"];

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
  return String(owm.city);
}

String owmDescription() {
  return String(owm.description);
}
