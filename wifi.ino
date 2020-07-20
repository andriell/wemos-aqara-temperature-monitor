// Дожидается подключения к wi-fi
// Выводит лог на экран
void wifiConnection()
{
  displayLog(String("Connecting to "));
  displayLog(String(configWifiSsid));

  while (WiFi.status() != WL_CONNECTED) {
    ledBlink(1, 250);
    displayLog(String("Connecting to Wi-Fi"));
  }

  displayLog(String("Wi-Fi connected"));
  displayLog(String("IP address: "));

  displayLog(wifiGetIpString());
  ledBlink(3, 500);
}

void wifiSetup() {
  WiFi.hostname("Temperature monitor");
  WiFi.mode(WIFI_STA);
  WiFi.begin(configWifiSsid, configWifiPassword);

  wifiConnection();

  WiFi.setAutoReconnect(true);
}

String wifiGetIpString()
{
  const IPAddress ipAddress = WiFi.localIP();
  return String(ipAddress[0]) + String(".") + \
         String(ipAddress[1]) + String(".") + \
         String(ipAddress[2]) + String(".") + \
         String(ipAddress[3]);
}


void wifiLoop()
{
  if (WiFi.status() != WL_CONNECTED) {
    displayClear();
    WiFi.reconnect();
    wifiConnection();
  }
}
