char incomingPacket[UDP_TX_PACKET_MAX_SIZE + 1];

struct StructWeather {
  String sid;
  long voltage;
  long temperature;
  long humidity;
  long pressure;
};

#define WEATHER_PERIOD 1801
int weatherLoopCount = WEATHER_PERIOD;

struct StructWeather weather1;
struct StructWeather weather2;

WiFiUDP weatherUdp;
WiFiUDP weatherUdpMulticast;
const IPAddress weatherUdpMulticastIp(224, 0, 0, 50);

void weatherSetup()
{
  weatherUdp.begin(9899);
  weatherUdpMulticast.beginMulticast(WiFi.localIP(), weatherUdpMulticastIp, 9898);
  weather1.sid = configWeatherSid1;
  weather2.sid = configWeatherSid2;
}

void weatherLoop()
{
  weatherRead(&weatherUdp);
  weatherRead(&weatherUdpMulticast);

  weatherLoopCount++;
  if (weatherLoopCount < WEATHER_PERIOD) {
    return;
  }
  weatherLoopCount = 0;

  weatherRequest(&weather1);
  weatherRequest(&weather2);
}

void weatherRequest(struct StructWeather* weather) {
  String request = "{\"cmd\":\"read\",\"sid\":\"" + weather->sid + "\"}";

  Serial.print("Send ");
  Serial.println(request);

  weatherUdp.beginPacket(configWeatherHubIp, 9898);
  weatherUdp.write((char*) request.c_str());
  int isEnd = weatherUdp.endPacket();

  Serial.print("End packet: ");
  Serial.println(isEnd);

}

void weatherRead(WiFiUDP* udp) {
  int packetSize = udp->parsePacket();
  if (!packetSize) {
    //Serial.println("Empty");
    return;
  }

  Serial.print("Packet. Size: ");
  Serial.print(packetSize);
  Serial.print("; Remote IP: ");
  Serial.print(udp->remoteIP().toString().c_str());
  Serial.print("; Remote Port: ");
  Serial.print(udp->remotePort());
  Serial.print("; Destination IP: ");
  Serial.print(udp->destinationIP());
  Serial.print("; Data: ");

  int len = udp->read(incomingPacket, UDP_TX_PACKET_MAX_SIZE);
  if (len > 0) {
    incomingPacket[len] = 0;
  }
  Serial.println(incomingPacket);

  // {"cmd":"read_ack","model":"weather.v1","sid":"***","short_id":1560,"data":"{\"voltage\":3005,\"temperature\":\"2571\",\"humidity\":\"3104\",\"pressure\":\"98738\"}"}
  // {"cmd":"report","model":"weather.v1","sid":"158d0003f1187f","short_id":7153,"data":"{\"temperature\":\"2497\"}"}
  // {"cmd":"report","model":"weather.v1","sid":"158d0003f1187f","short_id":7153,"data":"{\"humidity\":\"3349\"}"}
  // {"cmd":"report","model":"weather.v1","sid":"158d0003f1187f","short_id":7153,"data":"{\"pressure\":\"98780\"}"}

  DynamicJsonDocument doc(1024);

  deserializeJson(doc, incomingPacket);
  JsonObject response = doc.as<JsonObject>();
  String responseCmd = response["cmd"].as<String>();
  String responseSid = response["sid"].as<String>();
  String responseData = response["data"].as<String>();

  if (!(String("report").equals(responseCmd) || String("read_ack").equals(responseCmd))) {
    return;
  }

  StructWeather* weather;
  if (weather1.sid.equals(responseSid)) {
    weather = &weather1;
  } else if (weather2.sid.equals(responseSid)) {
    weather = &weather2;
  } else {
    return;
  }

  deserializeJson(doc, responseData);
  response = doc.as<JsonObject>();
  if (response.containsKey("voltage")) {
    weather->voltage = response["voltage"];
  }
  if (response.containsKey("temperature")) {
    weather->temperature = response["temperature"];
  }
  if (response.containsKey("humidity")) {
    weather->humidity = response["humidity"];
  }
  if (response.containsKey("pressure")) {
    weather->pressure = response["pressure"];
  }
  printWeather(weather);
}

void printWeather(struct StructWeather* weather) {
  Serial.print("Weather. Sid: ");
  Serial.print(weather->sid);
  Serial.print("; Voltage: ");
  Serial.print(weather->voltage);
  Serial.print("; Temperature: ");
  Serial.print(weather->temperature);
  Serial.print("; Humidity: ");
  Serial.print(weather->humidity);
  Serial.print("; Pressure: ");
  Serial.print(weather->pressure);
  Serial.println();
}

String weatherTemperature1() {
  return String(round(float(weather1.temperature) / 10.0) / 10.0, 1) + String(" °C");
}

String weatherTemperature2() {
  return String(round(float(weather2.temperature) / 10.0) / 10.0, 1) + String(" °C");
}

String weatherTemperature() {
  return String(round(float((weather1.temperature + weather2.temperature) / 2) / 10.0) / 10.0, 1) + String(" °C");
}

String weatherHumidity() {
  return String(round(float((weather1.humidity + weather2.humidity) / 2) / 10.0) / 10.0, 1) + String(" %");
}

String weatherPressure() {
  return String(round(float((weather1.pressure + weather2.pressure) / 2) / 100.0) / 10.0, 1) + String(" kPa");
}

long weatherVoltage1() {
  return weather1.voltage;
}

long weatherVoltage2() {
  return weather2.voltage;
}
