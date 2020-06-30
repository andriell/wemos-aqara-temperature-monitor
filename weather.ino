char incomingPacket[UDP_TX_PACKET_MAX_SIZE + 1];
char outputingPacket[1024];

WiFiUDP Udp;

void weatherSetup()
{
  Udp.begin(9898);
}


void weatherLoop()
{
  for (int i=0; i < 1024; i++){
    outputingPacket[i] = 0;
  }
  char first[] = "{\"cmd\":\"read\",\"sid\":\"";
  for (int i=0; i <= 21; i++){
    outputingPacket[i] = first[i];
  }
  for (int i=0; i < 14; i++){
    outputingPacket[i+21] = weatherSid1[i];
  }
  outputingPacket[35] = '"';
  outputingPacket[36] = '}';
  outputingPacket[37] = 0;
  
  Serial.print("Send ");
  Serial.println(outputingPacket);
  Udp.beginPacket(hubIp, 9898);
  Udp.write(outputingPacket);
  int isEnd = Udp.endPacket();
  Serial.print("End packet: ");
  Serial.println(isEnd);
  
  delay(1000);
  
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, UDP_TX_PACKET_MAX_SIZE);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
}
