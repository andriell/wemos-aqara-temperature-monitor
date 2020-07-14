// Network SSID

void wifiSetup() {
  Serial.println();
  Serial.println();

  displayLog(String("Connecting to "));
  displayLog(String(wifiSsid));


  WiFi.hostname("Temperature monitor");
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid, wifiPassword);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    ledBlink(1, 250);
    displayLog(String("Connecting ") + String(++i));
  }


  displayLog(String("WiFi connected"));
  displayLog(String("IP address: "));


  displayLog(wifiGetIpString());
  ledBlink(3, 500);
}

String wifiGetIpString()
{
  const IPAddress ipAddress = WiFi.localIP();
  return String(ipAddress[0]) + String(".") + \
         String(ipAddress[1]) + String(".") + \
         String(ipAddress[2]) + String(".") + \
         String(ipAddress[3]);
}
