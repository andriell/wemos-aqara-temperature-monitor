#define TIME_PERIOD 601
int timeLoopCount = TIME_PERIOD;


WiFiUDP timeUdp;

void timeSetup()
{
  timeUdp.begin(8888);
}

void timeLoop() {
  timeLoopCount++;
  if (timeLoopCount < TIME_PERIOD) {
    return;
  }
  timeLoopCount = 0;
  setTime(timeNtpTime());
}

String timeString()
{
  static char str[14];
  sprintf(str, "%02d:%02d:%02d %02d-%02d", hour(), minute(), second(), day(), month());
  return String(str);
}


/*-------- NTP code ----------*/

const int TIME_NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte timePacketBuffer[TIME_NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t timeNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (timeUdp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(configTimeServerName, ntpServerIP);
  Serial.print(configTimeServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  timeSendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = timeUdp.parsePacket();
    if (size >= TIME_NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      timeUdp.read(timePacketBuffer, TIME_NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)timePacketBuffer[40] << 24;
      secsSince1900 |= (unsigned long)timePacketBuffer[41] << 16;
      secsSince1900 |= (unsigned long)timePacketBuffer[42] << 8;
      secsSince1900 |= (unsigned long)timePacketBuffer[43];
      return secsSince1900 - 2208988800UL + configTimeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void timeSendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(timePacketBuffer, 0, TIME_NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  timePacketBuffer[0] = 0b11100011;   // LI, Version, Mode
  timePacketBuffer[1] = 0;     // Stratum, or type of clock
  timePacketBuffer[2] = 6;     // Polling Interval
  timePacketBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  timePacketBuffer[12] = 49;
  timePacketBuffer[13] = 0x4E;
  timePacketBuffer[14] = 49;
  timePacketBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  timeUdp.beginPacket(address, 123); //NTP requests are to port 123
  timeUdp.write(timePacketBuffer, TIME_NTP_PACKET_SIZE);
  timeUdp.endPacket();
}
