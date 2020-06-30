char incomingPacket[UDP_TX_PACKET_MAX_SIZE + 1];

WiFiUDP Udp;

struct StructWeather {
  String sid;
  long voltage;
  long temperature;
  long humidity;
  long pressure;
};

struct StructWeather weather1;
struct StructWeather weather2;

void weatherSetup()
{
  Udp.begin(9898);
  weather1.sid = weatherSid1;
  weather2.sid = weatherSid2;
}


void weatherLoop()
{
  updateWeather(&weather1);
  printWeather(&weather1, 1);
  updateWeather(&weather2);
  printWeather(&weather2, 2);
}

void updateWeather(struct StructWeather* weather) {
  String request = "{\"cmd\":\"read\",\"sid\":\"" + weather->sid + "\"}";

  Serial.print("Send ");
  Serial.println(request);

  Udp.beginPacket(hubIp, 9898);
  Udp.write((char*) request.c_str());
  int isEnd = Udp.endPacket();

  Serial.print("End packet: ");
  Serial.println(isEnd);

  delay(1000);

  int packetSize = Udp.parsePacket();
  if (!packetSize) {
    return;
  }

  // receive incoming UDP packets
  Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
  int len = Udp.read(incomingPacket, UDP_TX_PACKET_MAX_SIZE);
  if (len > 0)
  {
    incomingPacket[len] = 0;
  }
  Serial.printf("UDP packet contents: %s\n", incomingPacket);

  DynamicJsonDocument doc(1024);

  deserializeJson(doc, incomingPacket);
  JsonObject responce = doc.as<JsonObject>();
  String responceSid = responce["sid"];
  String responceData = responce["data"];
  if (!weather->sid.equals(responceSid)) {
    return;
  }
  deserializeJson(doc, responceData);
  responce = doc.as<JsonObject>();
  weather->voltage = responce["voltage"];
  weather->temperature = responce["temperature"];
  weather->humidity = responce["humidity"];
  weather->pressure = responce["pressure"];

  return;
}

void printWeather(struct StructWeather* weather, int i) {
  Serial.print("Weather: ");
  Serial.print(i);
  Serial.print("; Sid: ");
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
