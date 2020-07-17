#include "images.h"

#define img_x 72
#define img_y 9

SH1106 display(0x3C, WEMOS_D1, WEMOS_D2);

void displaySetup()   {
  display.init();
  display.flipScreenVertically();
  displayLog(String("Loading..."));
}

int icon = 1;
int spinner = 0;

void displayLoop() {
  display.clear();

  // Выравниваем надпись по центру второй половины
  // Средняя ширина одной буквы при размере шрифта 10 равна 5 px
  // При размере шрифта 10 в строке длинной 64 px помещается 15 символов
  display.setFont(Dialog_plain_8);
  String city = owmCity();
  if (city.length() >= 14) {
    display.drawString(64, 0, city);
  } else {
    display.drawString(64 + ((14 - city.length()) * 5) / 2, 0, city);
  }

  // Выравниваем надпись по центру второй половины
  // Средняя ширина одной буквы при размере шрифта 10 равна 5 px
  // При размере шрифта 10 в строке длинной 64 px помещается 13 символов
  display.setFont(ArialMT_Plain_10);
  String desc = owmDescription();
  if (desc.length() >= 12) {
    display.drawString(64, 54, desc);
  } else {
    display.drawString(64 + ((12 - desc.length()) * 5) / 2, 54, desc);
  }

  icon = owmIcon();
  switch (icon) {
    case 1:
      display.drawXbm(img_x, img_y, img_width, img_height, img01_bits);
      icon = 2;
      break;
    case 2:
      display.drawXbm(img_x, img_y, img_width, img_height, img02_bits);
      icon = 3;
      break;
    case 3:
      display.drawXbm(img_x, img_y, img_width, img_height, img03_bits);
      icon = 4;
      break;
    case 4:
      display.drawXbm(img_x, img_y, img_width, img_height, img04_bits);
      icon = 9;
      break;
    case 9:
      display.drawXbm(img_x, img_y, img_width, img_height, img09_bits);
      icon = 10;
      break;
    case 10:
      display.drawXbm(img_x, img_y, img_width, img_height, img10_bits);
      icon = 11;
      break;
    case 11:
      display.drawXbm(img_x, img_y, img_width, img_height, img11_bits);
      icon = 13;
      break;
    case 13:
      display.drawXbm(img_x, img_y, img_width, img_height, img13_bits);
      icon = 50;
      break;
    case 50:
      display.drawXbm(img_x, img_y, img_width, img_height, img50_bits);
      icon = 1;
      break;
  }


  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, weatherTemperature1());
  display.drawString(0, 22, weatherTemperature2());

  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 43, weatherHumidity());
  display.drawString(0, 52, weatherPressure());

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

  // Voltage
  // Две полоски в нижней части экрана.
  // За максимальный заряд батарейки принимаем 3000
  // За минималдьный 2500
  displayVoltage(weatherVoltage1(), 62);
  displayVoltage(weatherVoltage2(), 63);
  display.display();
}

// Voltage
// Две полоски в нижней части экрана.
// За максимальный заряд батарейки принимаем 3000
// За минималдьный 2500
void displayVoltage(long v, int y) {
  display.setPixel(51, y);
  long voltage = (v - 2500) / 10;
  Serial.print("Display. Voltage: ");
  Serial.print(voltage);
  Serial.print(" y: ");
  Serial.println(y);
  if (voltage > 0) {
    if (voltage > 50) {
      voltage = 50;
    }
    display.setPixel(voltage, y);
    //display.drawHorizontalLine(0, y, voltage);
  }
}

int displayLogPosition = 0;

// Вывод строчки лога на экран
void displayLog(String s) {
  Serial.println(s);
  if (displayLogPosition > 5) {
    displayLogPosition = 0;
    display.clear();
  }
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, displayLogPosition * 10, s);
  displayLogPosition++;
  
  display.display();
}

void displayClear() {
  displayLogPosition = 0;
  display.clear();
}
