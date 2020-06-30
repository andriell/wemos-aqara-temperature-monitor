#include "images.h"

SH1106 display(0x3C, WEMOS_D2, WEMOS_D1);

void displaySetup()   {
  display.init();
  display.flipScreenVertically();
}

int icon = 1;
int spinner = 0;

void displayLoop() {
  display.clear();

  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, owmCity());
  display.drawString(64, 54, owmDescription());

  icon = owmIcon();
  switch (icon) {
    case 1:
      display.drawXbm(64, 0, img_width, img_height, img01_bits);
      icon = 2;
      break;
    case 2:
      display.drawXbm(64, 0, img_width, img_height, img02_bits);
      icon = 3;
      break;
    case 3:
      display.drawXbm(64, 0, img_width, img_height, img03_bits);
      icon = 4;
      break;
    case 4:
      display.drawXbm(64, 0, img_width, img_height, img04_bits);
      icon = 9;
      break;
    case 9:
      display.drawXbm(64, 0, img_width, img_height, img09_bits);
      icon = 10;
      break;
    case 10:
      display.drawXbm(64, 0, img_width, img_height, img10_bits);
      icon = 11;
      break;
    case 11:
      display.drawXbm(64, 0, img_width, img_height, img11_bits);
      icon = 13;
      break;
    case 13:
      display.drawXbm(64, 0, img_width, img_height, img13_bits);
      icon = 50;
      break;
    case 50:
      display.drawXbm(64, 0, img_width, img_height, img50_bits);
      icon = 1;
      break;
  }


  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, weatherTemperature1());
  display.drawString(0, 22, weatherTemperature2());

  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 44, weatherHumidity());
  display.drawString(0, 54, weatherPressure());

  switch (spinner) {
    case 0:
      display.drawXbm(120, 0, 6, 6, planer_0_bits);
      break;
    case 1:
      display.drawXbm(120, 0, 6, 6, planer_1_bits);
      break;
    case 2:
      display.drawXbm(120, 0, 6, 6, planer_2_bits);
      break;
    default:
      display.drawXbm(120, 0, 6, 6, planer_3_bits);
      spinner = 0;
      break;
  }
  spinner++;

  display.display();
}
