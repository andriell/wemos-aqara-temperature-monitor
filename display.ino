#include "images.h"

SH1106 display(0x3C, WEMOS_D2, WEMOS_D1);

void displaySetup()   {
  display.init();
  display.flipScreenVertically();

  
}

void displayLoop() {
  display.clear();

  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, weatherTemperature1());

  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 16, weatherTemperature2());

  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 32, weatherHumidity());

  display.drawXbm(64, 0, img_width, img_height, img04_bits);
  
  display.display();
}
