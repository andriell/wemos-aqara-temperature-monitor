void ledSetup() {
  pinMode(WEMOS_D4_LED, OUTPUT);  // initialize onboard LED as output
  delay(10);
}

void ledBlink(int n, unsigned long d) {
  for (int i = 0; i <= n; i++) {
    digitalWrite(WEMOS_D4_LED, HIGH);  // turn on LED with voltage HIGH
    delay(d);                       // wait
    digitalWrite(WEMOS_D4_LED, LOW);   // turn off LED with voltage LOW
    delay(d);                       // wait
  }
}

void ledOff() {
  digitalWrite(WEMOS_D4_LED, LOW);
}
